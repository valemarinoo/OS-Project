
# Check if two filenames are provided
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <input_filename> <output_filename>"
    exit 1
fi

input_file="$1"
output_file="$2"

# Initialize memory addresses
address=0x4000000
read_address=0x4000320
output_string=" "

# Read integers from the input file and write to memory
while IFS= read -r integer; do
    busybox devmem $address w $integer
    address=$(printf "0x%X" $((address + 8)))
done < "$input_file"

# Execute the command once
busybox devmem 0x4000640 w 2

# Initialize a variable to track the number of characters read
char_count=0

# Read from memory and construct a string
while true; do
    # Read from memory and convert to character
    read_value=$(busybox devmem $read_address)
    char=$(printf "\\$(printf '%03o' "$read_value")")

    # Append character to the output string
    echo -n "$char" >> "$output_file"

    # Increment the read address by 8
    read_address=$(printf "0x%X" $((read_address + 8)))

    # Increment the character count
    char_count=$((char_count + 1))

    # Break if we've read enough characters (optional, depends on your use case)
    if [ "$char_count" -eq $(wc -l < "$input_file") ]; then
        break
    fi
done


# **License: [CC BY-NC 4.0](https://creativecommons.org/licenses/by-nc/4.0/)**