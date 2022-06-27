# reconcile-tape-files
Reconcile the Enstore file dump with SAM.

This program reads 2 or 3 input files and produces 4 output files.  The input files are:
Inputs:
1. A file containing a list of all Mu2e files in ENSTORE.  This can be obtained with the command: ```curl -O https://www-stken.fnal.gov/enstore/tape_inventory/COMPLETE_FILE_LISTING_mu2e``` .  You need to be on VPN or on site to download the file.  It is very large and will crash most browsers.  In the summer of 2022 it was 4.3 GB.
1. A file containing a summary of the Mu2e SAM datasets.  This file is available at: ```https://mu2e.fnal.gov/atwork/computing/ops/samMon.html```. 
1. Optional input: a text file containing the names of SAM datasets that were present in SAM when the second input file was made but which have since been deleted.  This is useful while doing ongoing work to purge datasets.  The file is allowed to be empty.

The output files are:

1. SAM_Listing_with_pnfs_counts.txt a copy of the second input file with additional information added.  See below for details
1. SAM_Listing_missing_with_pnfs_counts.txt the some information as the first output file but for SAM datasets that were identified in the ENSTORE listing but are not present in the second input file.
1. fileFamilySummary_allFiles.txt the number of files and the total file size sorted by file family for all files in the enstore listing
1. fileFamilySummary_currentFiles.txt the same information as the previous file but excluding datasets that were marked as deleted by the third input file.

The program reads the SAM dataset listing and keeps an internal copy.
It then processes the ENSTORE listing to count the number of files from each dataset that are present in the ENSTORE listing;
it also records the total size of those files.
It also creates internal dataset records for datasets not found in the SAM dataset listing and populates those with the file count and total size.

The columns in the first two output files have the following information:
1. From input: a code saying if where the files in this dataset are found; see the input file for a description.
2. From input: the number of SAM records in this dataset
3. From input: the number of files in this dataset
4. From input: the total size of files in this dataset (base 10 MB).
5. From input: the event count of files in this dataset; this is zero for files that do not contain events.
6. The number of files in this dataset that were found in the ENSTORE listing.
7. The total size of files in this dataset that were found in the ENSTORE listing (base 10 MB).
8. 1 if the dataset is present in the input list of deleted datsets; 0 otherwise.
9. A code saying where the files in this dataset are found; determined by processing the ENSTORE listing.  See below.
10. Column 6 minus column 3.
11. Column 8 minus column 4.
12. From input: the dataset name.

The code in column 9 has the following meaning:
- 0: no files were found in the ENSTORE listing
- 1: all files found in the enstore listing were in old-style /pnfs locations, ie not under /pnfs/mu2e/tape.
- 2: all files found in the enstore listing were in new-style /pnfs locations, ie under /pnfs/mu2e/tape.
- 3: files were found in the ENSTORE listing but locations were a mix of 1 and 2.

The last two output files the columns are:
1. The number of files found
2. The total size (base 10 MB).
3. The dataset name
