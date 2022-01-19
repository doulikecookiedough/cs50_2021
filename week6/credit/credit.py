from cs50 import get_string

# Default Card Type
ccnType = "INVALID"

# Retrieve Credit Card Number (ccn)
ccn = get_string("Number: ")

# If the ccn is greater than 16 digits, keep asking
while int(ccn) > 9999999999999999:
    ccn = get_string("Number: ")
    if int(ccn) < 9999999999999999:
        break

# Put ccn into a list
ccnNumList = [int(i) for i in str(ccn)]
# print(f"ccnNumList initial: {ccnNumList}")

# Determine if the list is first odd or even
oddEvenDetermination = len(ccn)
if (int(oddEvenDetermination) % 2) == 0: # even
    numTypeMod = 0 # Counts odd numbers
    numTypeNorm = 1
else: # odd
    numTypeMod = 1 # Counts even numbers
    numTypeNorm = 0

### For testing purposes ###
# print(f"numTypeMod: {numTypeMod}")
# print(f"numTypeNorm: {numTypeNorm}")

# Create a new list of numbers needing to be modified
modifiedNumList = []
for i in range(numTypeMod, len(ccnNumList), 2):
    modifiedNumList.append(ccnNumList[i])

# Modify list of numbers needing to be modified
modifiedNumListAfter = []
for number in modifiedNumList:
    m = number * 2
    if m > 9:
        for n in str(m):
            modifiedNumListAfter.append(int(n))
    else:
        modifiedNumListAfter.append(m)

# Calculate sum of modified number list
modifiedNumTotal = 0
for s in modifiedNumListAfter:
    modifiedNumTotal += int(s)

### For testing purposes ###
# print(f"modifiedNumList: {modifiedNumList}")
# print(f"modifiedNumListAfter: {modifiedNumListAfter}")
# print(f"modifiedNumTotal: {modifiedNumTotal}")

# Create a new list of numbers that don't need to be modified
normalNumList = []
for i in range(numTypeNorm, len(ccnNumList), 2):
    normalNumList.append(ccnNumList[i])

# Calculate sum of normal number list
normalNumTotal = 0
for s in normalNumList:
    normalNumTotal += int(s)

### For testing purposes ###
# print(f"normalNumList: {normalNumList}")
# print(f"normalNumTotal: {normalNumTotal}")

# If the total's last digit is 0, the number is valid
numTotal = 0
numTotal = normalNumTotal + modifiedNumTotal
if numTotal % 10 < 1: # Identify card type if valid
    # Now we do the check to get AMEX (34 or 37), VISA (4), MASTERCARD (51, 52, 53, 54, or 55)
    if ccnNumList[0] + ccnNumList[1] == 7:
        ccnType = "AMEX"
    if ccnNumList[0] + ccnNumList[1] == 10:
        ccnType = "AMEX"
    if ccnNumList[0] == 4:
        ccnType = "VISA"
    if ccnNumList[0] == 5:
        ccnType = "MASTERCARD"

print(ccnType)