# QT-POI
Data Analysis and Visualization of POI Checking-In Data for PPPS
## Build Requirements
```
Qt 5.9.9  MinGW 32
c++ & qml
```
## Load Data
Click "browsing" to select file from the disk   
(Wait a few minutes...)   
When the progress bar is loaded to 100%, it will show "upload successfull".   
Then click "OK, let's start!", jump to "welcome" page.   
<img src="https://github.com/Jzyz-Q/QT-POI/blob/main/image/open.png?raw=true" width="400px">

## Welcome and Filter
Set the range of data of interest to the user  
- User_ID / Location_ID: 1;3;4 means User_1 and User_3 and User_4;     
- Latitude / longitude: 33;35 means 33-35;   

View the introduction of each function   
<img src="https://github.com/Jzyz-Q/QT-POI/blob/main/image/filter.png?raw=true" width="400px">

## Spatio-temporal and Comparison Information
- Top POIs: show the ten most visited locations by bar charts
- Comprasion of Top10 POIs: compares the top10 of different users (Different colors represent different users, and the diameter of the dot represents the number of times the location has been visited)
- Number of Check-ins: show the number of times a certain location ID or a certain latitude and longitude range has been accessed by all users by line chart.
- Daily Active Users: compared the change in active users over time for two locations.(line graph: the number of times the location has been accessed at the current time step; pie charts: the distribution of users visiting)   
<img src="https://github.com/Jzyz-Q/QT-POI/blob/main/image/top.png?raw=true" width=400/>&emsp;<img src="https://github.com/Jzyz-Q/QT-POI/blob/main/image/cot.png?raw=true" width=400/>   
<img src="https://github.com/Jzyz-Q/QT-POI/blob/main/image/ckin.png?raw=true" width=400/>&emsp;<img src="https://github.com/Jzyz-Q/QT-POI/blob/main/image/dau.png?raw=true" width=400/>


