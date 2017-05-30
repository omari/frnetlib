#include <stdint.h>
#include <gtest/gtest.h>
#include <frnetlib/Packet.h>

TEST(PacketTest, pack_and_unpack_ints)
{
    fr::Packet packet;
    double a1 = 11.5f, a2 = 0.f;
    float b1 = 11.52, b2 = 0.0;
    uint8_t c1 = std::numeric_limits<uint8_t>::max(), c2 = 0;
    uint16_t d1 = std::numeric_limits<uint16_t>::max(), d2 = 0;
    uint32_t e1 = std::numeric_limits<uint32_t>::max(), e2 = 0;
    uint64_t f1 = std::numeric_limits<uint64_t>::max(), f2 = 0;

    packet << a1 << b1 << c1 << d1 << e1 << f1;
    packet >> a2 >> b2 >> c2 >> d2 >> e2 >> f2;

    ASSERT_EQ(a1, a2);
    ASSERT_EQ(b1, b2);
    ASSERT_EQ(c1, c2);
    ASSERT_EQ(d1, d2);
    ASSERT_EQ(e1, e2);
    ASSERT_EQ(f1, f2);

}

TEST(PacketTest, pack_and_unpack_stl)
{
    fr::Packet packet;
    std::string a1 = "I'm a string", a2;
    std::vector<std::string> b1 = {"hello", "there", "a"}, b2;
    std::pair<int, std::string> c1 = {1, "a"}, c2;

    packet << a1 << b1 << c1;
    packet >> a2 >> b2 >> c2;

    ASSERT_EQ(a1, a2);
    ASSERT_EQ(b1, b2);
    ASSERT_EQ(c1, c2);
}

TEST(PacketTest, variadic_packet_constructor)
{
    int a1 = 10, a2;
    std::string b1 = "hey", b2;
    int64_t c1 = 90, c2;

    fr::Packet packet(a1, b1, c1);
    packet >> a2 >> b2 >> c2;

    ASSERT_EQ(a1, a2);
    ASSERT_EQ(b1, b2);
    ASSERT_EQ(c1, c2);
}

TEST(PacketTest, raw_data)
{
    std::string a1 = "hello", a2;
    std::string b1(13, 'c'), b2(13, '\0');
    uint32_t c1 = std::numeric_limits<uint32_t>::max(), c2;
    fr::Packet packet;

    packet << a1;
    packet.add_raw(b1.c_str(), b1.size());
    packet << c1;

    packet >> a2;
    packet.extract_raw(&b2[0], b1.size());
    packet >> c2;

    ASSERT_EQ(a1, a2);
    ASSERT_EQ(b1, b2);
    ASSERT_EQ(c1, c2);
}

TEST(PacketTest, out_of_bounds_protection)
{
    int32_t a = 99;
    fr::Packet packet(a, a, a, a);
    packet >> a >> a >> a >> a;

    try
    {
        packet >> a;
        FAIL();
    }
    catch(const std::out_of_range &e)
    {

    }
}

TEST(PacketTest, read_cursor)
{
    int32_t a1 = 20, a2;
    std::string b1 = "hello", b2;
    fr::Packet packet(a1, b1);

    packet >> a2 >> b2;
    ASSERT_EQ(a1, a2);
    ASSERT_EQ(b1, b2);

    packet.reset_read_cursor();
    packet >> a2 >> b2;
    ASSERT_EQ(a1, a2);
    ASSERT_EQ(b1, b2);

    packet.reset_read_cursor((sizeof(a1)));
    packet >> b2;
    ASSERT_EQ(b1, b2);
}