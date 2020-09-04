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
    environment {
        PATH = "${workspace}/spinnaker_tools/tools:$PATH"
    }
    options {
        skipDefaultCheckout true
    }
    stages {
        stage('Clean and Checkout') {
            steps {
                echo "PATH is: $PATH"
                sh 'rm -rf ${WORKSPACE}/*'
                sh 'rm -rf ${WORKSPACE}/.[a-zA-Z0-9]*'
                dir('spinnaker_tools') {
                    checkout scm
                }
            }
        }
        
        stage('Build') {
            environment {
                SPINN_DIRS = "${workspace}/spinnaker_tools"
            }
            steps {
            
                sh 'echo $PATH'
                
                // Build with armcc
                sh 'make -C $SPINN_DIRS GNU=0'
                
                // Build SCAMP with armcc
                sh 'make -C $SPINN_DIRS/scamp GNU=0'
                
                // Build BMPC with armcc
                sh 'make -C $SPINN_DIRS/bmpc GNU=0'
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
