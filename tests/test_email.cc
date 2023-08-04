#include "helens/email/email.h"
#include "helens/email/smtp.h"

void test() {
    helens::EMail::ptr email = helens::EMail::Create(
            "user@163.com", "passwd",
            "hello world", "<B>hi xxx</B>hell world", {"564628276@qq.com"});
    helens::EMailEntity::ptr entity = helens::EMailEntity::CreateAttach("helens/helens.h");
    if(entity) {
        email->addEntity(entity);
    }

    entity = helens::EMailEntity::CreateAttach("helens/address.cc");
    if(entity) {
        email->addEntity(entity);
    }

    auto client = helens::SmtpClient::Create("smtp.163.com", 465, true);
    if(!client) {
        std::cout << "connect smtp.163.com:25 fail" << std::endl;
        return;
    }

    auto result = client->send(email, true);
    std::cout << "result=" << result->result << " msg=" << result->msg << std::endl;
    std::cout << client->getDebugInfo() << std::endl;
    //result = client->send(email, true);
    //std::cout << "result=" << result->result << " msg=" << result->msg << std::endl;
    //std::cout << client->getDebugInfo() << std::endl;
}

int main(int argc, char** argv) {
    helens::IOManager iom(1);
    iom.schedule(test);
    iom.stop();
    return 0;
}
