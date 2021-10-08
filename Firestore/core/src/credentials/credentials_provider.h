/*
 * Copyright 2018 Google
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FIRESTORE_CORE_SRC_CREDENTIALS_CREDENTIALS_PROVIDER_H_
#define FIRESTORE_CORE_SRC_CREDENTIALS_CREDENTIALS_PROVIDER_H_

#include <functional>
#include <string>

#include "Firestore/core/include/firebase/firestore/firestore_errors.h"
#include "Firestore/core/src/credentials/auth_token.h"
#include "Firestore/core/src/credentials/user.h"
#include "Firestore/core/src/util/statusor.h"
#include "absl/strings/string_view.h"

namespace firebase {
namespace firestore {
namespace credentials {

// `TokenErrorListener` is a listener that gets a token or an error.
using TokenListener = std::function<void(util::StatusOr<AuthToken>)>;

// Listener notified with a credential change.
using CredentialChangeListener = std::function<void(User user)>;

/**
 * Provides methods for getting and listening to authentication credentials.
 */
class CredentialsProvider {
 public:
  CredentialsProvider();

  virtual ~CredentialsProvider();

  /** Requests the current token. */
  virtual void GetToken(TokenListener completion) = 0;

  /**
   * Marks the last retrieved token as invalid, making the next `GetToken`
   * request force refresh the token.
   */
  virtual void InvalidateToken() = 0;

  /**
   * Sets the listener to be notified of credential changes (sign-in /
   * sign-out, token changes). It is immediately called once with the initial
   * credential.
   *
   * Call with nullptr to remove previous listener.
   */
  virtual void SetCredentialChangeListener(
      CredentialChangeListener change_listener) = 0;

 protected:
  /**
   * A listener to be notified of credential changes (sign-in / sign-out, token
   * changes). It is immediately called once with the initial credential.
   *
   * Note that this block will be called back on an arbitrary thread that is not
   * the normal Firestore worker thread.
   */
  CredentialChangeListener change_listener_;
};

}  // namespace credentials
}  // namespace firestore
}  // namespace firebase

#endif  // FIRESTORE_CORE_SRC_CREDENTIALS_CREDENTIALS_PROVIDER_H_