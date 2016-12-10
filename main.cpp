#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/scope_exit.hpp>
#include <gmp.h>
#include <gmpxx.h>

using boost::asio::ip::tcp;

enum { max_length = 1024 };// max_length - константа, что задает ограничение на вводимое число и ответ, и это приводит к ошибке при большей длине числа или большому выделению памяти

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(tcp::v4(), argv[1], argv[2]);
        tcp::resolver::iterator iterator = resolver.resolve(query);

        tcp::socket s(io_service);
        boost::asio::connect(s, iterator);

        using namespace std;
        for(;;){
            char reply[max_length];
            auto buf = make_shared<boost::asio::streambuf>();
            boost::asio::read_until(s, *buf, '\n');
            std::istream stream(buf.get());
             stream.getline(reply, max_length);
            std::cout << "Data recieve " << reply << std::endl;
            string str= reply;
            char *request = new char[max_length]();

            BOOST_SCOPE_EXIT((request)){
                delete[] request;
           } BOOST_SCOPE_EXIT_END

            if (reply[0]<=57 && reply[0] > 48) {// почему не использовать isdigit()?
                string ch = "";
                string st = "";
                auto found = str.find('^');
                ch = str.substr(0, found);
                st = str.substr(found + 1, str.size() - found);
                
                mpz_class ch1;
                mpz_class st1;
                mpz_class rez;
                mpz_init(rez.get_mpz_t());
                mpz_init(st1.get_mpz_t());
                mpz_init(ch1.get_mpz_t());
                mpz_set_str(st1.get_mpz_t(), st.c_str(), 10);
                mpz_set_str(ch1.get_mpz_t(), ch.c_str(), 10);

                rez = 1;
                for (int i = 1; i <= st1; i++) {
                    rez = rez * ch1;
                }

                mpz_get_str(request, 10, rez.get_mpz_t());
            }
            else {
                request[0] = '0';
            }
            string rezult=request;
            int length =rezult.size();
            request[length]='\n';
            boost::asio::write(s, boost::asio::buffer(request,length+1));
            if (request[0]=='0')
                break;
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
     return 0;
 }
