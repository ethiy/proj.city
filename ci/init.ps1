# Boost Download
New-Item -Type Directory -Path 'C:\Program Files\boost'
Invoke-WebRequest https://sourceforge.net/projects/boost/files/boost/1.62.0/boost_1_62_0.zip -UseBasicParsing -OutFile "C:\Program Files\boost\boost_1_62_0.zip"
7z x "C:\Program Files\boost\boost_1_62_0.zip" -o 'C:\Program Files\boost\boost_1_62_0'
Remove-Item 'C:\Program Files\boost\boost_1_62_0.zip'
'C:\Program Files\boost\boost_1_62_0\tools\build\bootsrap'
'C:\Program Files\boost\boost_1_62_0\tools\build\b2 install -j4 --build-type=minimal'
'C:\Program Files\boost\boost_1_62_0\bootsrap'
'C:\Program Files\boost\boost_1_62_0\tools\build\b2 install -j4 --build-type=minimal'

# CGAL Download
New-Item -Type Directory -Path 'C:\Program Files\CGAL\'
Invoke-WebRequest https://github.com/CGAL/cgal/releases/download/releases%2FCGAL-4.9/CGAL-4.9-Setup.exe -UseBasicParsing -OutFile "C:\Program Files\CGAL\CGAL-setup.exe"
'C:\Program Files\CGAL\CGAL-setup.exe'
