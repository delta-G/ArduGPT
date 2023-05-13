

####  Copyright 2023 David Caldwell disco47dave@gmail.com


#     This program is free software: you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by
#     the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.
# 
#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.
# 
#     You should have received a copy of the GNU General Public License
#     along with this program.  If not, see <http://www.gnu.org/licenses/>.


import serial, time

from langchain import OpenAI, PromptTemplate, LLMChain
from langchain.chat_models import ChatOpenAI


template = """
You are an LLM that is acting as a translator between human input and a home automation system.
You will be given a list of commands and arguments that each command can take. 
You will decide based on the human input which command and arguments to pass to the output and format them appropriately.

Here is a list of commands you can use with descriptions of their arguments and output:
[

command: "L"
description:  used to turn lights on or off
input:  either of "0" for off or "1" for on

command: "F"
description:  used to turn the fan on or off
input:  either of "0" for off or "1" for on

command: "A"
description:  used to set the thermostat for the HVAC system
input:  the temperature to set

]

If you do not find a suitable command, then respond with "unable"

To format the command string, place a "<" symbol before the command, then the command, then a comma "," and then any arguments as a comma separated list.  Finally include the ">" symbol.  

Here are some examples of formatted command strings:
To turn the light on: <L,1>
To turn the fan off: <F,0>
To set the HVAC to 85 degrees <A,85>

Be sure to include the formatted command string in your response and do not include any < or > symbols anywhere else in the response.  

Begin!

Human Input:
{input}

"""


class ArduGPT:
    
    def __init__(self, device):
        
        self.exit_flag = False
        self.device = device
        llm = ChatOpenAI(
            model_name="gpt-3.5-turbo", 
            temperature=0
            )

        prompt = PromptTemplate(
            input_variables=["input"],
            template=template,
            )   
        
        self.llm_chain = LLMChain(llm=llm, prompt=prompt)
        
        return
    
    
    def getUserInput(self):
        print("\n*******************************\n\nHuman:  ")
        self.human_input = input("")
        if self.human_input == "exit":
            self.exit_flag = True
        return 
    
    def printResponse(self):
        
        if self.ai_output is not None:
            print("\n*******************************\n\nAI:  ")
            print(self.ai_output)
            if 'unable' not in self.ai_output:
                self.device.write(bytes(self.ai_output, 'utf-8'))
                time.sleep(0.1)  ## sleep while we wait for response
                returned = self.device.readline()
                print("\n*******************************\n\nArduino:  ")
                print(returned)
            
        
        return 
    
    def getResponse(self):
            
        if self.human_input is not None:
            self.ai_output = self.llm_chain.run(self.human_input)                             
        
        return 
    
    def run(self):
        
        self.getUserInput()
        while not self.exit_flag:
            self.getResponse()
            self.printResponse()
            self.getUserInput()   
        return 
    
arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=0.1)
    

ard = ArduGPT(arduino)
ard.run()
