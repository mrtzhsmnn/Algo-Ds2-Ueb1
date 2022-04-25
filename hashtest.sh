g++ -o hashtest hashtest.cxx

for x in c l q d; do
    echo $x
    ./hashtest 8 $x q <<- END
	p 0 4 null
	p 1 3 eins
	p 2 4 zwei
	p 3 3 drei
	r 0 4
	d
	q
	END
done
