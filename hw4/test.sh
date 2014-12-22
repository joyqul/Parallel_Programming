make origin 
make wave

./cuda_wave_origin 20 20 > tcase.ans
./cuda_wave 20 20 > tcase.out
echo '=====' > result
echo 'compare tcas1' >> result
diff tcase.ans tcase.out >> result

./cuda_wave_origin 200 200 > tcase.ans
./cuda_wave 200 200 > tcase.out
echo '=====' >> result
echo 'compare tcas2' >> result
diff tcase.ans tcase.out >> result

time ./cuda_wave 1000000 1000000 > tcase.out
time ./cuda_wave_origin 1000000 1000000 > tcase.ans
echo '=====' >> result
echo 'compare tcas2' >> result
diff tcase.ans tcase.out >> result

echo '=====' >> result
less result
