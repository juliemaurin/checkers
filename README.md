# Checkers++
The goal of this project is to create a checkers-playing robot. This project is divided in several folders :

 - **Game** : C++ Port of [Checkers](https://github.com/Ziink4/checkers), a simple game engine for playing [English Draughts](https://en.wikipedia.org/wiki/English_draughts)
 - **Vision** : [OpenCV](https://opencv.org)-based software tool used to acquire information about a checkerboard from a camera.
 - **Control** : All the necessary software to control the robot arm that will play the game physically.

*More information is available in each subdirectory.*













#Instruction Manual 

**Used Material**

-UR10 robotic arm 
-Gripper
-Camera & Camera tripod
-Computer with Linux distribution
-Modem WIFI to connect the Computer with the gripper & the UR10 arm

**Software Requirements**
-Installing Python 
-Installing urx
.. To be continued

**Launching the program**

1. Robot Calibration:
	cd Control	
	python game_server.py	

2. Camera Calibration:
	cd Vision
	./checkersvision videoCalib
            (empty board image --> full borad image ---> select the borad borders from the bottom-left corner moving clockwise)

	./checkersvision videoParse thresh_value
		(The thresh_value is generated in the calibration command)

3. Start the Game:
	cd Game
	./checkerspp walter vision 
		(Player Vs Robot --> White VS Black in our case)

4. The robot starts playing. When it is moving, the player has to stay out of the security area. After finishing its turn, the player can begin its move. When the player finishes (clears the security area), the vision system sends an order to the robot allowing it to make its move. 
The security area has to be clear while the robot is moving. Otherwise, the robot stops moving for security purposes.


#External references

https://www.youtube.com/watch?v=STjW3eH0Cik

https://chrislarson1.github.io/blog/2016/05/30/cnn-checkers/

https://webdocs.cs.ualberta.ca/~chinook/databases/

http://www.learningmachines101.com/build-machine-learns-play-checkers/

http://aurora.shashki.com/

http://ieeexplore.ieee.org/document/809083/

https://github.com/mkoryak/evolutionary-neural-net-ai/tree/master/checkerAI

##
> Written with [StackEdit](https://stackedit.io/).
