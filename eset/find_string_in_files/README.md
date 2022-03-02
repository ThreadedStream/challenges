## Description of the challenge

Input: The application will receive two command line parameters. The first parameter is a path to a file or folder, while the second parameter is the string to be searched for (maximum length of 128 characters).

Output: Print the output to stdout lines in format "file(position): prefix…suffix".

file: Name of a file containing the string

position: Offset within the file where the string started (zero based).

prefix: Contains three characters in front of the match

suffix: Contains three characters after the match.

When there are not enough characters for the whole prefix or suffix (at the beginning or end of the file), use as many as there are available. In the prefixes and suffixes, tabs should be printed as “\t” and new lines as “\n”.

Assume that the path can point to either a single text file, or to a directory with files and/or more folders (your application must be able to recursively traverse and process all files in sub-directories). The application must take into account that files might be very large (potential memory constraints/limitations). The application must not crash for any reason (ensure that exceptions are handled properly, handling of errors from API functions, etc), and should print meaningful errors messages when a problem occurs (informing when there was not enough arguments provided, input path is invalid, etc).

Note! The solution should make use of multi-threading in processing data (at least theoretically). Please note that a single threaded solution may be faster in this sequential scan, especially on slower drives. Try to maximize speed and compare your results with a single threaded solution.

