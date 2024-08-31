#!/bin/bash

# Define the log file location
LOGFILE="./daemon_output.log"

# Get the current date and time
NOW=$(date)

# Write a message to the log file
echo "Script executed at $NOW" >> $LOGFILE
