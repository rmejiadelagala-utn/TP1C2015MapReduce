function crearConfigJob {
	echo IP=$1 > Job/job1.cfg
	echo 	PORT=4000 >> Job/job1.cfg
	echo 	MAPPER=../Scripts/Weather/mapper.sh >> Job/job1.cfg
	echo 	REDUCE=../Scripts/Weather/reduce.sh >> Job/job1.cfg
	echo 	COMBINER=SI >> Job/job1.cfg
	echo 	ARCHIVOS=[/mr/weather/201301hourly.txt,/mr/weather/201302hourly.txt,/mr/weather/201303hourly.txt,/mr/weather/201304hourly.txt] >> Job/job1.cfg
	echo 	RESULTADO=/output/job1/max-temps.txt >> Job/job1.cfg
	echo 	IP=$1 > Job/job2.cfg
	echo 	PORT=4000 >> Job/job2.cfg
	echo 	MAPPER=../Scripts/WordCount/map.py >> Job/job2.cfg
	echo 	REDUCE=../Scripts/WordCount/reduce.py >> Job/job2.cfg
	echo 	COMBINER=SI >> Job/job2.cfg
	echo 	ARCHIVOS=[/mr/textos/gutenberg.txt,/mr/textos/linux.txt] >> Job/job2.cfg
	echo 	RESULTADO=/output/job2y3/textos-comb.txt >> Job/job2.cfg
	echo 	IP=$1 > Job/job3.cfg
	echo 	PORT=4000 >> Job/job3.cfg
	echo 	MAPPER=../Scripts/WordCount/map.py >> Job/job3.cfg
	echo 	REDUCE=../Scripts/WordCount/reduce.py >> Job/job3.cfg
	echo 	COMBINER=NO >> Job/job3.cfg
	echo 	ARCHIVOS=[/mr/textos/gutenberg.txt,/mr/textos/linux.txt] >> Job/job3.cfg
	echo 	RESULTADO=/output/job2y3/textos-nocomb.txt >> Job/job3.cfg
	echo 	IP=$1 > Job/job4.cfg
	echo 	PORT=4000 >> Job/job4.cfg
	echo 	MAPPER=../Scripts/LetterCount/mapper >> Job/job4.cfg
	echo 	REDUCE=../Scripts/LetterCount/reducer >> Job/job4.cfg
	echo 	COMBINER=SI >> Job/job4.cfg
	echo 	ARCHIVOS=[/mr/textos/gutenberg.txt,/mr/textos/linux.txt,/mr/textos/kernel.txt] >> Job/job4.cfg
	echo 	RESULTADO=/output/job4/rep-letras.tx >> Job/job4.cfg
	echo 	IP=$1 > Job/job5.cfg
	echo 	PORT=4000 >> Job/job5.cfg
	echo 	MAPPER=../Scripts/Twitter/basic_sentiment_analysis.py >> Job/job5.cfg
	echo 	REDUCE=../Scripts/Twitter/top-sent.pl >> Job/job5.cfg
	echo 	COMBINER=NO >> Job/job5.cfg
	echo 	ARCHIVOS=[/sentiment/tweet-1.csv,/sentiment/tweet-2.csv,/sentiment/tweet-3.csv,/sentiment/tweet-4.csv,/sentiment/tweet-5.csv,/sentiment/tweet-6.csv,/sentiment/tweet-7.csv,/sentiment/tweet-8.csv,/sentiment/tweet-9.csv,/sentiment/tweet-10.csv,/sentiment/tweet-11.csv,/sentiment/tweet-12.csv,/sentiment/tweet-13.csv] >> Job/job5.cfg
	echo 	RESULTADO=/output/job5/tweets-sent.csv >> Job/job5.cfg
}

function crearConfigFS {
	echo 	PUERTO_FS=3000 > Filesystem/Debug/ConfigFS.cfg
}

function crearConfigMarta {
	echo 	PORT=4000 > Marta/marta.cfg
	echo 	IP_FS=$1 >> Marta/marta.cfg
	echo 	PUERTO_FS=3000 >> Marta/marta.cfg
}

function crearConfigNodos {
for ((i=1;i<=7;i++))
do 
	truncate -s 2048M Nodo/Archivos/data$i.bin
	echo "PUERTO_FS="3000 > Nodo/Archivos/ConfigNodo$i.txt
	echo "IP_FS="$1 >> Nodo/Archivos/ConfigNodo$i.txt
	echo "ARCHIVO_BIN=data"$i".bin" >> Nodo/Archivos/ConfigNodo$i.txt
	echo "DIR_TEMP=/TMP" >> Nodo/Archivos/ConfigNodo$i.txt
	echo "NODO_NUEVO="SI >> Nodo/Archivos/ConfigNodo$i.txt
	VAR_IP_NODO=`ip route get 1 | awk '{print $NF;exit}'`
	echo "IP_NODO=$VAR_IP_NODO" >> Nodo/Archivos/ConfigNodo$i.txt
	echo "PUERTO_NODO=500"$i >> Nodo/Archivos/ConfigNodo$i.txt
	echo "ID="$i >> Nodo/Archivos/ConfigNodo$i.txt
done
}

function crearConfigs {
	ipLocal=
	echo "Ingrese la IP del Filesystem"
	read ipFS
	echo "Ingrese la IP de Marta"
	read ipMarta
	crearConfigJob $ipMarta
	crearConfigFS
	crearConfigMarta $ipFS
	crearConfigNodos $ipFS
}

function makear {
	cd Socketes/Debug
	make clean
	make all
	sudo cp libsocketes.so /usr/lib/libsocketes.so
	cd ..
	sudo cp -r socketes/ /usr/include
	cd ../Filesystem/Debug
	make clean
	make all
	cd ../../Marta/Debug
	make clean
	make all
	cd ../../Nodo/Debug
	make clean
	make all
	cd ../../Job/Debug
	make clean
	make all
	cd ../..
}




crearConfigs
makear
sudo apt-get cowsay

echo "Deploy realizado exitosamente" | cowsay -f stegosaur
