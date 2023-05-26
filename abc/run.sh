g++ grader/manager.cpp -std=c++11 -o manager -O2 &
g++ grader/stub.cpp abc.cpp -std=c++11 -o stub -O2 &
wait
echo "Compile OK"

for IN in `ls -d ./tests/*.in`
do
    echo "Running on $IN"

    export FIFOS="alice.out alice.in bob.out bob.in circuit.out circuit.in"
    rm -f $FIFOS
    mkfifo $FIFOS
    killall manager stub
    echo "mkfifo OK"

    ./manager $FIFOS < $IN &
    ./stub 0 < alice.in > alice.out &
    ./stub 1 < bob.in > bob.out &
    ./stub 2 < circuit.in > circuit.out &
    wait

    echo "===================="
done
