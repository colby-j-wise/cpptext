#!/bin/bash

# time on commandline or in script
# write to new output 
start=$(date +%s)
for filename in ./data/blogs/*.xml; #./runtime_testcases/*.txt
do
	[ -e "$filename" ] || continue
	name=$(basename $filename)

	# remove blank lines
	
	< "$filename" awk 'NF > 0'  |

	# Optional - remove regex i.e. <html> (missing multi whitespace)
	awk -v RS='<[^>]+>' -v ORS= '1' |

	# lowercase all files
    awk '{print tolower($0)}' | 

    # remove punctuation (replaces with "")
    awk '{ gsub(/[[:punct:]]/, "", $0) } 1;' |

    # remove digits
    tr -d 0-9  > "./runtime_output_awk/$name" 

    # Optional: - remove stop words
          
done

# Print run time
end=$(date +%s) 
runtime=$(python -c "print '%u:%02u' % ((${end} - ${start})/60, (${end} - ${start})%60)")
echo "Runtime was $runtime"

