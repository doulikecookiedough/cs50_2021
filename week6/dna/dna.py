import sys
import csv

# Wrap program in main function for cleanliness
def main():

    # Exit the program if 3 arguments are not provided
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")
        exit(1)

    # Read database file into memory
    with open(sys.argv[1], "r") as databaseFile:
        reader = csv.DictReader(databaseFile)
        # Return dictionary
        db_list = list(reader)

    # Read sequence file into memory
    with open(sys.argv[2], "r") as sequenceFile:
        # Return string
        sequence = sequenceFile.read()

    # Store sequences into empty array
    sequence_counts = []

    # Calculate the longest repeated sequence
    for i in range(1, len(reader.fieldnames)): # Starts on 1 as 0 stores names of persons
        STR = reader.fieldnames[i] # STR to check for matches
        sequence_counts.append(0) # Start all STR counts at 0
        # Loop through sequence file to find STRs
        for j in range(len(sequence)):
            STR_count = 0
            # Count repeats if a match is found
            # - Returns a new string with just the characters of 'sequence' (where 'sequence' is a string)
            # - ... starting from character 'j' up through (but not including) character 'j + len(STR)'
            if sequence[j:(j + len(STR))] == STR:
                k = 0
                while sequence[(j + k):(j + k + len(STR))] == STR:
                    STR_count += 1
                    k += len(STR)
                # If there is a new maximum of repeats, update sequence_counts
                if STR_count > sequence_counts[i - 1]:
                    sequence_counts[i - 1] = STR_count

    # Compare our sequence data to list
    for i in range(len(db_list)):
        matches = 0
        for j in range(1, len(reader.fieldnames)):
            if int(sequence_counts[j - 1]) == int(db_list[i][reader.fieldnames[j]]):
                matches += 1
            if matches == (len(reader.fieldnames) - 1):
                print(db_list[i]['name'])
                exit(0)

    print("No match")

main()