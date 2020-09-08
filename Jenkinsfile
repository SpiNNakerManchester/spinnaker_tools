/*
# Copyright (c) 2017-2019 The University of Manchester
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
        
        stage('Build') {
            environment {
                SPINN_DIRS = "${workspace}/spinnaker_tools"
                WORKSPACE = "${workspace}"
                PERL5LIB = "${workspace}/spinnaker_tools/tools:$PERL5LIB"
            }
            steps {
                
                // Build with armcc
                catchError {
                    sh 'make GNU=0 -C $SPINN_DIRS'
                }
                
                // Build SCAMP with armcc
                catchError {
                    sh 'PATH="$WORKSPACE/spinnaker_tools/tools:$PATH" make GNU=0 -C $SPINN_DIRS/scamp'
                    sh 'make GNU=0 -C $SPINN_DIRS/scamp install'
                }
                
                // Build BMP with armcc
                catchError {
                    sh 'PATH="$WORKSPACE/spinnaker_tools/tools:$PATH" make -C $SPINN_DIRS/bmp'
                }
            }
        }
        
        stage('Boot') {
            environment {
                PERL5LIB = "${workspace}/spinnaker_tools/tools:$PERL5LIB"
            }
            steps {
                // Boot a 3-board machine
                sh 'spalloc 3 -c "hwtests/board_tests/boot-bt {}"'
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
