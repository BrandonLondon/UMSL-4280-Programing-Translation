data _MIN := 1; 
data _MAX := 99; 
data _N := 0; 
data a := 0; 
data b := 1; 
data sum := 0; 
main
begin
    void input;
    getter _N;
    if [_N =< _MIN - 1] then
        proc input;
    ;
    if [_N => _MAX + 1] then
        proc input;
    ;

    void fib;
    if [_N [==] 0] then
        begin
            outter a;
            
            assign sum := a + b;
            assign a := b;
            assign b := sum;

            assign _N := _N - 1;

            proc fib;
        end
    ;
end
