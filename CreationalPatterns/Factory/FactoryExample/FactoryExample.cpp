#include <vector>
#include <string>
#include <gtest/gtest.h>

namespace FactoryTester {

    struct Person
    {
        unsigned int id;
        std::string name;
    };

    class PersonFactory
    {
        unsigned int id{ 0 };
    public:
        Person create_person(const  std::string& name)
        {
            return { id++, name };
        }
    };

    namespace
    {
        class Evaluate : public testing::Test
        {
        };

        TEST_F(Evaluate, SimplePersonTest)
        {
            PersonFactory factory;

            auto pointer1 = factory.create_person("Chris");
            ASSERT_EQ("Chris", pointer1.name);

            auto pointer2 = factory.create_person("Sarah");
            ASSERT_EQ(1, pointer2.id) << "Expected the second created person's id to be = 1";
        }
    } // namespace

}

int main(int ac, char* av[])
{
    testing::InitGoogleTest(&ac, av);

    return RUN_ALL_TESTS();
}
