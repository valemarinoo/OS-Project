
# Check if two filenames are provided
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <input_filename> <output_filename>"
    exit 1
fi

input_file="$1"
output_file="$2"

# Read string from the input file
input_string=$(cat "$input_file")

# Check if the input string is empty
if [ -z "$input_string" ]; then
    echo "Input file is empty"
    exit 1
fi

address=0x4000000
read_address=0x4000320

# Initialize index for while loop
i=0

# Loop over each character of the string
while [ $i -lt ${#input_string} ]; do
    char=$(printf "%d" "'${input_string:$i:1}'")
    
    # Write the character to the specified memory address
    busybox devmem $address w $char

    # Increment the address by 8
    address=$(printf "0x%X" $((address + 8)))

    # Increment index
    i=$((i + 1))
done

# Execute the command once
busybox devmem 0x4000640 w 1

> $output_file
# Reset index for next loop
i=0

# Read from memory and write to file for each character
while [ $i -lt ${#input_string} ]; do
    # Read from memory
    read_value=$(busybox devmem $read_address)

    # Write the read value to the output file
    echo $read_value >> $output_file

    # Increment the read address by 8
    read_address=$(printf "0x%X" $((read_address + 8)))

    # Increment index
    i=$((i + 1))
done


# **License: [CC BY-NC 4.0](https://creativecommons.org/licenses/by-nc/4.0/)**