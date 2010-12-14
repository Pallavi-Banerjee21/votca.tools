/* 
 * Copyright 2010 The VOTCA Development Team (http://www.votca.org)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "easylock.h"

namespace votca { namespace tools {

EasyLock::EasyLock(Mutex *mutex) {
  // TODO Auto-generated constructor stub
  myMutex = mutex;
  myMutex->Lock();
}

EasyLock::~EasyLock() {
  // TODO Auto-generated destructor stub
  myMutex->Unlock();
}

}}
