# Copyright (c) 2016 The University of Manchester
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

APPS = aggregator tracer
GNU := 1

all:
	for app in $(APPS); do ("$(MAKE)" -f Makefile.spinn GNU=$(GNU) APP=$$app) || exit $$?; done
	"$(MAKE)" -f Makefile.host

clean:
	for app in $(APPS); do ("$(MAKE)" -f Makefile.spinn GNU=$(GNU) APP=$$app clean) || exit $$?; done
	"$(MAKE)" -f Makefile.host clean
