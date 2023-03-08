/*
# Copyright (c) 2017 The University of Manchester
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
*/
pipeline {
    agent {
        docker { 
            image 'python3.6'
            args '--privileged'
        }
    }
    options {
        skipDefaultCheckout true
    }
    stages {
        stage('Clean and Checkout') {
            steps {
                sh 'rm -rf ${WORKSPACE}/*'
                sh 'rm -rf ${WORKSPACE}/.[a-zA-Z0-9]*'
                dir('spinnaker_tools') {
                    checkout scm
                }
            }
        }

        stage('Setup') {
            steps {
                sh 'pip install spalloc'
                dir('hwtests') {
                    git branch:'master', credentialsId: '98413a50-3a5d-4ca9-b672-5bfc168f01a5', url:'https://github.com/SpiNNakerManchester/SpiNNaker_hardware_tests'
                }
                sh 'mkdir ~/.config'
                sh 'echo "[spalloc]" > ~/.config/spalloc'
                sh 'echo "hostname = spinnaker.cs.man.ac.uk" >> ~/.config/spalloc'
                sh 'echo "owner = Jenkins" >> ~/.config/spalloc'
            }
        }

        stage('ARMCC Build and Test') {
            environment {
                SPINN_DIRS = "${workspace}/spinnaker_tools"
                SPINN_PATH = "${workspace}/hwtests/board_tests"
                WORKSPACE = "${workspace}"
                PERL5LIB = "${workspace}/spinnaker_tools/tools:$PERL5LIB"
            }
            steps {

                // Build base and SCAMP with armcc
                catchError {
                    // show compiler version
                    sh 'armcc --vsn'

                    sh 'make GNU=0 -C $SPINN_DIRS'
                    sh 'PATH="$WORKSPACE/spinnaker_tools/tools:$PATH" make GNU=0 -C $SPINN_DIRS/scamp'
                    sh 'make GNU=0 -C $SPINN_DIRS/scamp install'

                    // Boot a SpiNN-5 board
                    sh 'spalloc -c hwtests/board_tests/boot-bt {}'
                }

                // Build BMP with armcc
                catchError {
                    sh 'PATH="$WORKSPACE/spinnaker_tools/tools:$PATH" make -C $SPINN_DIRS/bmp'
                }
            }
        }

        stage('GCC Build and Test') {
            environment {
                SPINN_DIRS = "${workspace}/spinnaker_tools"
                SPINN_PATH = "${workspace}/hwtests/board_tests"
                WORKSPACE = "${workspace}"
                PERL5LIB = "${workspace}/spinnaker_tools/tools:$PERL5LIB"
            }
            steps {

                // show compiler version
                sh 'arm-none-eabi-gcc --version'

                // Build base and SCAMP with gcc
                sh 'make GNU=1 -C $SPINN_DIRS'
                sh 'PATH="$WORKSPACE/spinnaker_tools/tools:$PATH" make GNU=1 -C $SPINN_DIRS/scamp'
                sh 'make GNU=1 -C $SPINN_DIRS/scamp install'

                // Boot a SpiNN-5 board
                sh 'spalloc -c hwtests/board_tests/boot-bt {}'

                // Cannot build BMP with gcc so far
            }
        }
    }
    post {
        always {
            script {
                emailext subject: '$DEFAULT_SUBJECT',
                    body: '$DEFAULT_CONTENT',
                    recipientProviders: [
                        [$class: 'CulpritsRecipientProvider'],
                        [$class: 'DevelopersRecipientProvider'],
                        [$class: 'RequesterRecipientProvider']
                    ],
                    replyTo: '$DEFAULT_REPLYTO'
            }
        }
    }
}
