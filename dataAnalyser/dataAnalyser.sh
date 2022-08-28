#TODO
#extract threads from terminal run (integrate into run.sh ("-a", "-c"))
#if already a file, change name

declare -a training
declare -a setting
declare -a threads
declare -a tempTimeLogArray
mediumTime=0
stdDev=0

training=("letter" "miniboone" "SDD" "SkinSeg" "statlog") #"susy")
setting=("list" "listavx" "listsimd") #"parallel" "paravx" "parsimd" "parseq")
threads=("1" "6" "12" "18" "24") #"30" "36" "42" "48" "54" "60" "66" "72")
executions=10

for loop1 in "${training[@]}"
do
	for loop2 in "${setting[@]}"
	do
		for loop3 in "${threads[@]}"
		do
			tempTimeLogArray=()
			file=$(echo "../results/logs/"$loop1"-"$loop2"-"$loop3".log")
			grep -f filter.txt $file > temp1.txt
			sed 's/Learning time: //g' temp1.txt > temp2.txt
			tempTimeLog=$(sed 's/ seconds//g' temp2.txt)
			tempTimeLogArray=($tempTimeLog)

			#echo ${tempTimeLogArray[*]}

			sum=$(dc -e "0 ${tempTimeLogArray[*]/-/_} ${tempTimeLogArray[*]/*/+} p")
			mediumTime=$(echo "scale=10; $sum / $executions" | bc -l)
			#echo $mediumTime

			stdDev=$(
				echo "${tempTimeLogArray[*]}" | 
					awk '{
						sum=0;ssq=0;
						for(i=1; i<=5; i++){
							sum+=$i;
							ssq+=($i*$i);
						}
						print 2*(sqrt(ssq/5-(sum/5)*(sum/5)))
					}'
				)
			#echo $stdDev

			case $loop1 in
			letter) nloop1='1' ;;
			miniboone) nloop1='2' ;;
			SDD) nloop1='3' ;;
			SkinSeg) nloop1='4' ;;
			statlog) nloop1='5' ;;
			susy) nloop1='6' ;;
			*) echo "Error" ;;
			esac

			case $loop2 in
			list) nloop2='1' ;;
			listavx) nloop2='2' ;;
			listsimd) nloop2='3' ;;
			parallel) nloop2='4' ;;
			paravx) nloop2='5' ;;
			parsimd) nloop2='6' ;;
			parseq) nloop2='7' ;;
			*) echo "Error" ;;
			esac

			if [ "$mediumTime" != "" ]
			then
				echo "$nloop1	$nloop2	$loop3	$mediumTime	$stdDev" >> execLog.txt
			else
				echo "$nloop1	$nloop2	$loop3	"0"	"0"" >> execLog.txt
			fi
			rm temp1.txt
			rm temp2.txt
		done
	done
done

