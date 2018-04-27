import os, sys
import re
import string
from time import time


# remove using regex
def removeRegex(line, regex_list):
    for pattern in regex_list:
        line = re.sub(pattern," ",line)
    return line

# remove digits
def removeDigit(line):
    return ''.join([word for word in line if not word.isdigit()])

# remove stop words
def remove_stopwords(line, stop_words):
    return ''.join([word for word in line if word not in stop_words])




input_path = 'data/blogs/'  #'runtime_testcases/'
output_dir = 'runtime_output/'

regex_list = [ "<[^>]+>", "(?m)^[^\S\n]+"]
stop_words = ["post"]
translator = str.maketrans('', '', string.punctuation)

start = time()
for _file in os.listdir(input_path):
    _ = os.path.join(input_path, _file)
    with open(_) as fp:
        try:
            for line in fp:
                # 1: skip blank lines
                if not line.isspace():  
                    # Optional: remove regex
                    #line = removeRegex(line, regex_list)
                    # 2: remove whitespace
                    line = line.strip()
                    # 3: lowercase
                    line = line.lower()
                    # 4: remove punctuation
                    line = line.translate(translator)
                    # 5: remove digits
                    line = removeDigit(line)
                    # 6: remove stop words
                    line = removeRegex(line, stop_words)
                    line = line.rstrip() # removes \r
                    with open( os.path.join(output_dir, "pytest_"+_file), "a" ) as outFile:
                        outFile.write(line+'\n') # add new line
        except:
            continue
end = time()
print ('Elapsed runtime: {}'.format(end-start))        