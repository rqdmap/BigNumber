all: main data

main: main.cpp BigDecimal.h BigDecimal.cpp BigNumber.h BigNumber.cpp rqdmap.h
	g++ main.cpp BigDecimal.h BigDecimal.cpp BigNumber.h BigNumber.cpp rqdmap.h -o main -D TIME
data: data.cpp

tester: tester.cpp
	g++ tester.cpp -o tester

clean:
	rm data main
