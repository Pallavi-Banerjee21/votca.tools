# 
# Copyright 2009 The VOTCA Development Team (http://www.votca.org)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
_votca_common_opts()
{
  local cur myopt 
  cur=${COMP_WORDS[COMP_CWORD]}
  if [[ "$cur" == --* ]]; then
     myopt=$( $1 --help 2> /dev/null | sed  -e '/--/!d' \
     -e 's/.*?\(--[A-Za-z0-9]\+\).*/\1/' | sort -u )
     COMPREPLY=( $( compgen -W '$myopt' -- $cur ) )
   fi
}

