#include "test_data_send.h"
#include "test_client_server.h"

int main()
{
    test_data_send_not_file();
    test_data_send_1_file();
    test_data_send_2_file();

    test_client_server_connect_disconnect();
    test_client_server_check_server();
    test_client_server_file();
    return 0;
}
