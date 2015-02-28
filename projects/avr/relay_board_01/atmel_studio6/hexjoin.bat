type %1 | findstr /v :00000001FF > tmp.hex
type %2 >> tmp.hex
move tmp.hex %3