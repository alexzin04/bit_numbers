#include <iostream>
#include <vector>
#include <cmath>



//знаковые числа интервалы при 8 битах -128,127
class BinaryNumber{
    bool overflow_flag;
    int bitness;
    std::vector<int> bits;
   public:
        BinaryNumber(int num,int bit){
            bitness=bit;
            overflow_flag=false;
            bool isnegative=false;
            if (num<0){
                num=-num;
                isnegative=true;
            }
            if (num>=std::pow(2,bitness-1)){
                throw ("Переполнение");
            }
            for(int i=0;i<bitness;i++){
                    bits.push_back(num % 2);
                    num /= 2;
                }
                std::reverse(bits.begin(),bits.end());
            if(isnegative){
                for(int i=0;i<bitness;i++){
                    bits[i]=bits[i]?0:1;
                }
                int c=1;
                for(int i=bitness-1;i>=0;i--){
                    bits[i]+=c;
                    c=bits[i]/2; 
                    bits[i]%=2;
                }
            }
        }
        BinaryNumber(){};
        BinaryNumber(std::vector<int>  bits_new,int bit ,bool c_flag=false){
            bitness=bit;
            overflow_flag=c_flag;
            for(int i=0;i<bitness;i++){
                   bits.push_back(bits_new[i]);
                }
        }
        std::vector<int> to_additional_code(int bitness,std::vector<int> bits){
            for(int i=0;i<bitness;i++){
                bits[i]=bits[i]?0:1;
            }
            int c=1;
            for(int i=bitness-1;i>=0;i--){
                bits[i]+=c;
                c=bits[i]/2; 
                bits[i]%=2;
            }
            return bits;
        }
        int get_bitness(){return bitness;}
        void print(){
            std::vector<int> bits_new(bits);
            std::cout<<"Вывод в двоичном формате"<<std::endl;
            for(int i=0;i<bitness;i++){
                    std::cout<<bits_new[i];
                }
            std::cout<<std::endl<<"Вывод десетичного числа"<<std::endl;
            bool flag=bits_new[0];
            long int sum=0;
            if(flag){
                int c=1;
                for(int i=bitness-1;i>=0;i--){
                    bits_new[i]=bits_new[i]^c;
                    c=bits_new[i]&&c;
                }
                for(int i=0;i<bitness;i++){
                    bits_new[i]=bits_new[i]?0:1;
                }
                for(int i=bitness-1;i>=0;i--){
                    if (bits_new[i]){
                    sum+=std::pow(2,bitness-i-1);
                    }
                }
                sum=-sum;
            }
            else{
                for(int i=bitness-1;i>=0;i--){
                    if (bits[i]){
                    sum+=std::pow(2,bitness-i-1);
                    }
                }
            }
            std::cout<<sum<<std::endl;
        }
        BinaryNumber operator+(BinaryNumber& other) {
            //проверка на размер если размер не равен исключение
            bool flag;
            if (bitness!=other.get_bitness()){
                throw "Операторы разной размерности";
            }
            std::vector<int> bit_res(bitness,0);
            char bit_1,bit_2,bit_c=0,sum;
            for(int i=bitness-1;i>=0;i--){
                bit_1=bits[i];
                bit_2=other.bits[i];
                sum=bit_1+bit_2+bit_c;
                bit_res[i]=sum%2;
                bit_c=sum/2;
            }
            if(bits[0]==other.bits[0] && bit_res[0]==!bits[0]){
                flag=true;
                throw "Произошло переполнение";
            
            }
            return BinaryNumber(bit_res,bitness,flag);
        }
        BinaryNumber operator*(BinaryNumber&  other ) {
            if (bitness!=other.get_bitness()){
                throw "Операторы разной размерности";
            }
            std::vector<int>result(bitness*2, 0),bits_1(bits),bits_2(other.bits);
            bool sigma_res=bits[0]^other.bits[0]; 
            if(bits[0]){
                bits_1=to_additional_code(bitness,bits_1);
            }

            if(other.bits[0]){
                bits_2=to_additional_code(other.get_bitness(),bits_2);
            }
            
            // Умножение каждого бита числа a на каждый бит числа b
            for (int i = bitness - 1; i >= 0; --i) {
                for (int j = bitness - 1; j >= 0; --j) {
                    result[i + j + 1] += bits_1[i]  * bits_2[j];
                }
            }
            // Обработка переносов
            for (int i = bitness*2 - 1; i > 0; --i) {
                if (result[i] > 1) {
                    int carry = result[i] / 2;
                    result[i] = result[i]  % 2;
                    result[i - 1] += carry;
                }
            }
            if(sigma_res){
                result=to_additional_code(bitness*2,result);
            }
            
            return BinaryNumber(result,bitness*2,false);
        }
        BinaryNumber operator-(BinaryNumber& other) {
            if (bitness!=other.get_bitness()){
                throw "Операторы разной размерности";
            }
            std::vector<int> bit_2= to_additional_code(other.get_bitness(),other.bits);//делает доп код
            BinaryNumber b(bit_2,other.get_bitness());// создает other=-other
            return *this+b;//op1+(-op2)
        }
};

void check_bitnes(int bitness){
     if (bitness != 8 && bitness != 16 && bitness != 32) {
        std::cout<<"Указана неверная размерность, завершение программы"<<std::endl;
        exit(1);
    }
}

int main(){
    int bitness_1,number_1,bitness_2,number_2;
    BinaryNumber num_1,num_2,num_3;
    char operation;
    std::cout<<"Введите размерность 1 числа"<<std::endl;
    std::cin>>bitness_1;
    check_bitnes(bitness_1);
    std::cout<<"Введите 1 число"<<std::endl;
    std::cin>>number_1;
    try{
        num_1= BinaryNumber(number_1,bitness_1);
    }
    catch(const char * err){
        std::cout<<err;//обработка переполнения, число не помешается в размерность
        exit(1);
    }
    std::cout<<"Введите размерность 2 числа"<<std::endl;
    std::cin>>bitness_2;
    check_bitnes(bitness_2);
    std::cout<<"Введите 2 число"<<std::endl;
    std::cin>>number_2;
    try{
        num_2= BinaryNumber(number_2,bitness_2);
    }
    catch(const char * err){
        std::cout<<err;//обработка переполнения, число не помешается в размерность
        exit(1);
    }
    std::cout<<"Введите операцию +,-,*"<<std::endl;
    std::cin>>operation;
    if(operation!='+' && operation!='-' && operation!='*' ){
        std::cout<<"Указана неверная операция, завершение программы"<<std::endl;
        exit(1);
    }
    try{
        if(operation=='+'){
            num_3=num_1+num_2;
        }
        if(operation=='-'){
            num_3=num_1-num_2;
        }
        if(operation=='*'){
            num_3=num_1*num_2;
        }
    }
    catch(const char * err){
        std::cout<<err;//обработка ошибок при выаполнии операции
        exit(1);
    }
    num_3.print();
}   
