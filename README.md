```
usage: ./glbl [options] [good_file [...]] <bad_file>

options:

  -c FILE  load configuration from FILE

  -g       omit good lines
  -b       omit bad lines
  -s       omit seen lines

  -d       debug: show the normalized strings
  -h       show this help

description:

This tool reads zero or more 'good' files followed by one 'bad' file.
All line of the files are normalized by removing substrings that match
list of given regular expressions. When parsing the 'bad' file, lines
are classified into three groups that are displayed in different colors:

  good: lines that were seen in one of the 'good' files

  bad:  lines that are were not seen in one of the 'good' files

  seen: 'bad' lines that are seen more then once

If the -d option is not given a limited fixed list of patterns is used,
dropping common timestamps, decimal and hexidecimal numbers
```
