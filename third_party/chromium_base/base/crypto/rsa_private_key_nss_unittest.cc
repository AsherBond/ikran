// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/crypto/rsa_private_key.h"

#include <keyhi.h>
#include <pk11pub.h>

#include "base/nss_util.h"
#include "base/scoped_ptr.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace base {

class RSAPrivateKeyNSSTest : public testing::Test {
 public:
  RSAPrivateKeyNSSTest() {}
  virtual ~RSAPrivateKeyNSSTest() {}

  virtual void SetUp() {
#if defined(OS_CHROMEOS)
    base::OpenPersistentNSSDB();
#endif
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(RSAPrivateKeyNSSTest);
};

TEST_F(RSAPrivateKeyNSSTest, FindFromPublicKey) {
  // Create a keypair, which will put the keys in the user's NSSDB.
  scoped_ptr<base::RSAPrivateKey> key_pair(base::RSAPrivateKey::Create(256));

  std::vector<uint8> public_key;
  ASSERT_TRUE(key_pair->ExportPublicKey(&public_key));

  scoped_ptr<base::RSAPrivateKey> key_pair_2(
      base::RSAPrivateKey::FindFromPublicKeyInfo(public_key));

  EXPECT_EQ(key_pair->key_->pkcs11ID, key_pair_2->key_->pkcs11ID);
}

TEST_F(RSAPrivateKeyNSSTest, FailedFindFromPublicKey) {
  // Create a keypair, which will put the keys in the user's NSSDB.
  scoped_ptr<base::RSAPrivateKey> key_pair(base::RSAPrivateKey::Create(256));

  std::vector<uint8> public_key;
  ASSERT_TRUE(key_pair->ExportPublicKey(&public_key));

  // Remove the keys from the DB, and make sure we can't find them again.
  if (key_pair->key_) {
    PK11_DestroyTokenObject(key_pair->key_->pkcs11Slot,
                            key_pair->key_->pkcs11ID);
  }
  if (key_pair->public_key_) {
    PK11_DestroyTokenObject(key_pair->public_key_->pkcs11Slot,
                            key_pair->public_key_->pkcs11ID);
  }

  EXPECT_EQ(NULL, base::RSAPrivateKey::FindFromPublicKeyInfo(public_key));
}

}  // namespace base
