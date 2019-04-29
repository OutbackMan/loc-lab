#! /usr/bin/python

import os
import sys

starting_abs_path = sys.argv[1]
path_to_remove = os.path.dirname(starting_abs_path)

if __name__ == "__main__":
    for cur_dir, _, files in os.walk(starting_abs_path):
        rel_cur_dir_name = cur_dir.replace(path_to_remove, "")
        for file_name in files:
            print os.path.join(rel_cur_dir_name, file_name)
