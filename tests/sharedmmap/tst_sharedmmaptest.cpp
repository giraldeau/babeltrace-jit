#include <QString>
#include <QtTest>

#include <iostream>
#include <memory>

struct Container {
    Container() {
        std::cout << "Container ctor" << std::endl;
    }
    ~Container() {
        std::cout << "Container dtor" << std::endl;
    }
};

struct Temp {
    Temp(std::shared_ptr<Container> data) : m_data(data) {
        std::cout << "Temp ctor " << m_data.use_count() << std::endl;
    }
    ~Temp() {
        std::cout << "Temp dtor " << m_data.use_count() << std::endl;
    }
    std::shared_ptr<Container> m_data;
};

class SharedmmapTest : public QObject
{
    Q_OBJECT

public:
    SharedmmapTest();

private Q_SLOTS:
    void testCase1();
};

SharedmmapTest::SharedmmapTest()
{
}

void SharedmmapTest::testCase1()
{
    std::shared_ptr<Container> data = std::make_shared<Container>();
    std::cout << data.use_count() << std::endl;
    Temp tmp(data);
    std::cout << tmp.m_data.use_count() << std::endl;
    data.reset();
    std::cout << tmp.m_data.use_count() << std::endl;
    std::cout << "end of testCase1" << std::endl;
}

QTEST_APPLESS_MAIN(SharedmmapTest)

#include "tst_sharedmmaptest.moc"
