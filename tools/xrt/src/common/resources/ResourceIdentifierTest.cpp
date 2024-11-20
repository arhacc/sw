#include <common/resources/ResourceIdentifier.hpp>

#include <gtest/gtest.h>

TEST(ResourceIdentifierTest, FromToString) {
    auto ri1str = "foo.bar.buz.hex@12.4.9#4880766485de7821db13940395a913c8";
    auto ri1 = ResourceIdentifier::fromString(ri1str);
    auto ri1gm = ResourceIdentifier{
        .package = {"foo", "bar"},
        .name = "buz",
        .fileType = ResourceIdentifier::FileType::Hex,
        .version = ResourceIdentifier::Version{
            .major = 12,
            .minor = 4,
            .patch = 9
        },
        .hash = {
          static_cast<uint8_t>('\x48'),
          static_cast<uint8_t>('\x80'),
          static_cast<uint8_t>('\x76'),
          static_cast<uint8_t>('\x64'),
          static_cast<uint8_t>('\x85'),
          static_cast<uint8_t>('\xde'),
          static_cast<uint8_t>('\x78'),
          static_cast<uint8_t>('\x21'),
          static_cast<uint8_t>('\xdb'),
          static_cast<uint8_t>('\x13'),
          static_cast<uint8_t>('\x94'),
          static_cast<uint8_t>('\x03'),
          static_cast<uint8_t>('\x95'),
          static_cast<uint8_t>('\xa9'),
          static_cast<uint8_t>('\x13'),
          static_cast<uint8_t>('\xc8')
        }
    };

    EXPECT_EQ(ri1, ri1gm) << fmt::format("Resource identifier {} failed to pasre", ri1str);
    EXPECT_EQ(ri1gm.toString(), ri1str) << fmt::format("Resource identifier {} failed to stringify", ri1str);

    auto ri2str = "a.so@0.0.0#4880766485de7821db13940395a913c8";
    auto ri2 = ResourceIdentifier::fromString(ri2str);
    auto ri2gm = ResourceIdentifier{
        .package = {},
        .name = "a",
        .fileType = ResourceIdentifier::FileType::So,
        .version = ResourceIdentifier::Version{
            .major = 0,
            .minor = 0,
            .patch = 0
        },
        .hash = {
          static_cast<uint8_t>('\x48'),
          static_cast<uint8_t>('\x80'),
          static_cast<uint8_t>('\x76'),
          static_cast<uint8_t>('\x64'),
          static_cast<uint8_t>('\x85'),
          static_cast<uint8_t>('\xde'),
          static_cast<uint8_t>('\x78'),
          static_cast<uint8_t>('\x21'),
          static_cast<uint8_t>('\xdb'),
          static_cast<uint8_t>('\x13'),
          static_cast<uint8_t>('\x94'),
          static_cast<uint8_t>('\x03'),
          static_cast<uint8_t>('\x95'),
          static_cast<uint8_t>('\xa9'),
          static_cast<uint8_t>('\x13'),
          static_cast<uint8_t>('\xc8')
        }
    };

    EXPECT_EQ(ri2, ri2gm) << fmt::format("Resource identifier {} failed to pasre", ri2str);
    EXPECT_EQ(ri2gm.toString(), ri2str) << fmt::format("Resource identifier {} failed to stringify", ri2str);

    auto ri3str = "a.b.onnx@1.2.31313#4880766485de7821db13940395a913c8";
    auto ri3 = ResourceIdentifier::fromString(ri3str);
    auto ri3gm = ResourceIdentifier{
        .package = {"a"},
        .name = "b",
        .fileType = ResourceIdentifier::FileType::Onnx,
        .version = ResourceIdentifier::Version{
            .major = 1,
            .minor = 2,
            .patch = 31313
        },
        .hash = {
          static_cast<uint8_t>('\x48'),
          static_cast<uint8_t>('\x80'),
          static_cast<uint8_t>('\x76'),
          static_cast<uint8_t>('\x64'),
          static_cast<uint8_t>('\x85'),
          static_cast<uint8_t>('\xde'),
          static_cast<uint8_t>('\x78'),
          static_cast<uint8_t>('\x21'),
          static_cast<uint8_t>('\xdb'),
          static_cast<uint8_t>('\x13'),
          static_cast<uint8_t>('\x94'),
          static_cast<uint8_t>('\x03'),
          static_cast<uint8_t>('\x95'),
          static_cast<uint8_t>('\xa9'),
          static_cast<uint8_t>('\x13'),
          static_cast<uint8_t>('\xc8')
        }
    };

    EXPECT_EQ(ri3, ri3gm) << fmt::format("Resource identifier {} failed to pasre", ri3str);
    EXPECT_EQ(ri3gm.toString(), ri3str) << fmt::format("Resource identifier {} failed to stringify", ri3str);

    auto ri4str = "my.my.xpu_tensor@0.0.0#4880766485de7821db13940395a913c8";
    auto ri4 = ResourceIdentifier::fromString(ri4str);
    auto ri4gm = ResourceIdentifier{
        .package = {"my"},
        .name = "my",
        .fileType = ResourceIdentifier::FileType::Tensor,
        .version = ResourceIdentifier::Version{
            .major = 0,
            .minor = 0,
            .patch = 0
        },
        .hash = {
          static_cast<uint8_t>('\x48'),
          static_cast<uint8_t>('\x80'),
          static_cast<uint8_t>('\x76'),
          static_cast<uint8_t>('\x64'),
          static_cast<uint8_t>('\x85'),
          static_cast<uint8_t>('\xde'),
          static_cast<uint8_t>('\x78'),
          static_cast<uint8_t>('\x21'),
          static_cast<uint8_t>('\xdb'),
          static_cast<uint8_t>('\x13'),
          static_cast<uint8_t>('\x94'),
          static_cast<uint8_t>('\x03'),
          static_cast<uint8_t>('\x95'),
          static_cast<uint8_t>('\xa9'),
          static_cast<uint8_t>('\x13'),
          static_cast<uint8_t>('\xc8')
        }
    };
    
    EXPECT_EQ(ri4, ri4gm) << fmt::format("Resource identifier {} failed to pasre", ri4str);
    EXPECT_EQ(ri4gm.toString(), ri4str) << fmt::format("Resource identifier {} failed to stringify", ri4str);
}

TEST(ResourceIdentifierTest, FromStringExceptions) {
    auto ri1str = ".hex@12.4.9#4880766485de7821db13940395a913c8";
    EXPECT_THROW(ResourceIdentifier::fromString(ri1str), BadResourceIdentifierFormatException)
        << fmt::format("Expected error when parsing {}; reason: missing name", ri1str);

    auto ri2str = "aa.aa@12.4.9#4880766485de7821db13940395a913c8";
    EXPECT_THROW(ResourceIdentifier::fromString(ri2str), BadResourceIdentifierFormatException)
        << fmt::format("Expected error when parsing {}; reason: bad extension", ri2str);

    auto ri3str = ".aa.hex@12.4.9#4880766485de7821db13940395a913c8";
    EXPECT_THROW(ResourceIdentifier::fromString(ri3str), BadResourceIdentifierFormatException)
        << fmt::format("Expected error when parsing {}; reason: missing package part", ri3str);

    auto ri4str = "aa..aa.hex@12.4.9#4880766485de7821db13940395a913c8";
    EXPECT_THROW(ResourceIdentifier::fromString(ri4str), BadResourceIdentifierFormatException)
        << fmt::format("Expected error when parsing {}; reason: missing package part", ri4str);

    auto ri5str = "aa.aa.@12.4.9#4880766485de7821db13940395a913c8";
    EXPECT_THROW(ResourceIdentifier::fromString(ri5str), BadResourceIdentifierFormatException)
        << fmt::format("Expected error when parsing {}; reason: missing extension", ri5str);

    auto ri6str = "aa.aa.hex@12.4#4880766485de7821db13940395a913c8";
    EXPECT_THROW(ResourceIdentifier::fromString(ri6str), BadResourceIdentifierFormatException)
        << fmt::format("Expected error when parsing {}; reason: bad version", ri6str);

    auto ri7str = "aa.aa.hex@alpha#4880766485de7821db13940395a913c8";
    EXPECT_THROW(ResourceIdentifier::fromString(ri7str), BadResourceIdentifierFormatException)
        << fmt::format("Expected error when parsing {}; reason: bad version", ri7str);

    auto ri8str = "aa.aa.hex@0.0.-1#4880766485de7821db13940395a913c8";
    EXPECT_THROW(ResourceIdentifier::fromString(ri8str), BadResourceIdentifierFormatException)
        << fmt::format("Expected error when parsing {}; reason: bad version", ri8str);

    auto ri9str = "aa.aa.hex@0.0a.0#4880766485de7821db13940395a913c8";
    EXPECT_THROW(ResourceIdentifier::fromString(ri9str), BadResourceIdentifierFormatException)
        << fmt::format("Expected error when parsing {}; reason: bad version", ri9str);

    auto ri10str = "aa.aa.hex@0.0.0#4880766485de7821db13940395a913c";
    EXPECT_THROW(ResourceIdentifier::fromString(ri10str), BadResourceIdentifierFormatException)
        << fmt::format("Expected error when parsing {}; reason: hash too short", ri10str);

    auto ri11str = "aa.aa.hex@0.0.0#4880766485de7821db13940395a913ccc";
    EXPECT_THROW(ResourceIdentifier::fromString(ri11str), BadResourceIdentifierFormatException)
        << fmt::format("Expected error when parsing {}; reason: hash too long", ri11str);

    auto ri12str = "aa.aa.hex@0.0.0#4880766485de7821db13940395a913cg";
    EXPECT_THROW(ResourceIdentifier::fromString(ri12str), BadResourceIdentifierFormatException)
        << fmt::format("Expected error when parsing {}; reason: non-hex character in has", ri12str);

    auto ri13str = "aa.aa.hex@0.0.0#";
    EXPECT_THROW(ResourceIdentifier::fromString(ri13str), BadResourceIdentifierFormatException)
        << fmt::format("Expected error when parsing {}; reason: missing hash", ri13str);
}
