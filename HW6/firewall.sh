#!/bin/bash



input_rules_file="$1"
# Save the packets passed as stdin via txt file
packets_input=$(cat)
#using grep to avoid inserting comments inside the file.
filtered_rules=$(grep -Ev '^#|^$' "$input_rules_file")

#cleaning the comments from the file
cleaned_rules=$(echo "$filtered_rules" | sed 's/#.*//')

#Divide the rules into an array, with each line representing a set of rules.
IFS=$'\n' read -rd '' -a rules_array <<< "$cleaned_rules"
# loop through each line in the provided relevant rules.
for line in "${rules_array[@]}"; do
#utilize awk to tokenize the line using commas and whitespace as delimiters.
    IFS=$'\n' read -rd '' -a rules_set <<< "$(echo "$line" |
                                       tr -d   '[:space:]' |
      awk -F '[,[:space:]]+' '{for (i=1; i<=NF; i++) print $i}')"
    #apply the rules to the packets
#the output is a subset of packets satisfying all four rules in the line.
    legal_packets+="$(echo "$packets_input" |
     ./firewall.exe "${rules_set[0]}" |
     ./firewall.exe "${rules_set[1]}" |
     ./firewall.exe "${rules_set[2]}" |
     ./firewall.exe "${rules_set[3]}" | sed 's/ //g')"$'\n'
done

# Sort and eliminate duplicates from the legal packets
legal_packets=$(echo "$legal_packets" | sed '/^[[:space:]]*$/d' | sort -u)
echo "$legal_packets"






