#Description
This is a program to classify input voices as One or Six
It also generates cropped clips of the utterances, based on a threshold value(configurable via a variable).

#How to run the code
1. Compile main.cpp (Important: use the '-std=c++11' option else it wont compile)
	g++ -std=c++11 -o main main.cpp
2. Run main.exe
	main.exe  <duration_in_seconds>
3. When it shows 'Start Recording...' you can start speaking. You can speak many utterances until it shows 'Stop Recording.'. Then press any key to continue. The program will separate them and tell for each of them if it was a ONE or a SIX.
All the utterances found will be saved in the 'output_clips/' folder with their labels.

========================================================================
    CONSOLE APPLICATION : one-six recognition Project Overview
========================================================================

After running the application, you need to put the duration of recording as the argument, the suggested value is 2 (in seconds).
The program creates two files named "recording.wav" and "recording.txt" which contains the recording data.
Then the required processing is applied and the file "saved_recording.txt" is generated which has the extracted voice data and on further which the ZCR values are calculated. 

Input Format: Input the duration of recording (in seconds) and then speak in the microphone at the prompt. Then press return(or any key to continue).
Output Format: Result showing wheather the number spoken is ONE/SIX.

Note: The recording should be performed in nosieless and without ambient noise background. Also the numbers should be spoken loudly and clearly.
