# k-means Alogrithm
Classifys the the speech signal into seperate classes. Used for classifying vowels.

Input:
The Universe file with the Cepstral Coefficient values of the recordings. To run replace the current "myUniverse.txt" file with custom universe path or keep the same file.

current myUniverse file has first 5 Ci values of 'a', then next 5 Cis of 'e' and so on.
Initial yi values are taken as follows. Total Universe size is 250.
top 50 values centroid -> Class 0 
next 50 values centroid -> Class 1 
next 50 values centroid -> Class 2 
next 50 values centroid -> Class 3 
next 50 values centroid -> Class 4 

Output:
Shows the Distortions after each iteration, utill 30 iterations or if the values converge before .
Finally shows the words assigned to the classes after completion.
Current file (myUniverse) has first 5 'a' then next 5 'e' ... and so on.
