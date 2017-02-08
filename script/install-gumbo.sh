mkdir tmp 
if curl https://github.com/google/gumbo-parser/archive/master.zip > tmp/master.zip
	then
		if unzip tmp/master.zip
			then
				cd tmp/master
				./autogen.sh
				./configure
				make
				sudo make install
				cd ../..
			else
				echo "[error] zip"
		fi
	else
		echo "[error] download"
fi
rm -rf tmp