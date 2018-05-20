// Copyright 2018 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include <algorithm>
#include <array>
#include <gtest/gtest.h>

#include "Common/Crypto/ec.h"

constexpr std::array<u8, 30> PRIVATE_KEY{{0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9,
                                          0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9,
                                          0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9}};

constexpr std::array<u8, 60> PUBLIC_KEY{
    {0x00, 0x21, 0x5b, 0xf7, 0x48, 0x2a, 0x64, 0x4b, 0xda, 0x9e, 0x02, 0x87, 0xaa, 0x37, 0x7d,
     0x0c, 0x5d, 0x27, 0x48, 0x72, 0xf1, 0x19, 0x45, 0x44, 0xdf, 0x74, 0x57, 0x67, 0x60, 0xcd,
     0x00, 0xa8, 0x6c, 0xe8, 0x55, 0xdd, 0x52, 0x98, 0x95, 0xc5, 0xc3, 0x3f, 0x7b, 0x0f, 0xc6,
     0x9f, 0x95, 0x8b, 0x3e, 0xe3, 0x33, 0x84, 0x2f, 0x32, 0xe9, 0x03, 0xe6, 0xfb, 0xc8, 0x51}};

TEST(ec, Sign)
{
  static constexpr std::array<u8, 20> HASH{{0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9,
                                            0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9}};
  const std::array<u8, 60> sig = Common::ec::Sign(PRIVATE_KEY.data(), HASH.data());
  // r and s must be non-null.
  EXPECT_FALSE(std::all_of(sig.cbegin(), sig.cbegin() + 30, [](u8 b) { return b == 0; }));
  EXPECT_FALSE(std::all_of(sig.cbegin() + 30, sig.cend(), [](u8 b) { return b == 0; }));
}

TEST(ec, PrivToPub)
{
  EXPECT_EQ(Common::ec::PrivToPub(PRIVATE_KEY.data()), PUBLIC_KEY);
}

TEST(ec, GenerateSharedSecret)
{
  static constexpr std::array<u8, 60> SECRET = {
      {0x01, 0x20, 0x2b, 0x3b, 0x63, 0x18, 0x5b, 0x2f, 0x05, 0x4f, 0xb5, 0x2c, 0xe5, 0x46, 0xc2,
       0x2d, 0x4e, 0x73, 0xf4, 0x15, 0xcb, 0xd2, 0x56, 0x7f, 0xff, 0x3f, 0x02, 0x23, 0xbe, 0xda,
       0x01, 0xf3, 0x0c, 0x34, 0xb6, 0x37, 0xbf, 0x55, 0x5b, 0x04, 0x49, 0x5a, 0x07, 0xee, 0x78,
       0xd2, 0x9a, 0x31, 0xce, 0x10, 0x42, 0xbf, 0x79, 0xc3, 0xcb, 0x22, 0x40, 0xe5, 0x94, 0x7f}};

  EXPECT_EQ(Common::ec::ComputeSharedSecret(PRIVATE_KEY.data(), PUBLIC_KEY.data()), SECRET);
}