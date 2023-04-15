#!/bin/bash
#Set the URL to scrape
site="https://www.ynetnews.com/category/3082"
#Scrape the page and extract article links
data=$(wget --no-check-certificate -O - "$site" 2>/dev/null)
#Find the relevant articles according to the demand type of links
articles=$(echo "$data" | \
grep -oP "https://(www.)?ynetnews.com/article/[0-9a-zA-Z]+" | sort | uniq)
echo "$(echo "$articles" | wc -l)" >> results.csv
echo "$articles" | while read  url; do
  relevant_article="$url"
  #Initialize the string variable
  results=""
 Link_data=$(wget --no-check-certificate -O - "$relevant_article" 2>/dev/null)
 #Loop through the names and add them to the string if they are found
 for name in "Netanyahu" "Gantz" "Bennett"; do
    count_name=$(echo -n "$Link_data" | grep -o --color=always "$name" | wc -l)
    results=$results$name", "$count_name", "
 done
    results=$results"Peretz, "$(echo "$Link_data" | grep -o "Peretz" | wc -l)
 if [ "$results" = "Netanyahu, 0, Gantz, 0, Bennett, 0, Peretz, 0" ]; then
		  results=" -"
 fi
 echo "$relevant_article", "$results" >> results.csv
done
