from flask import Flask, jsonify, request, Response, make_response
from flask_cors import CORS

server = Flask(__name__)
CORS(server=server)

class SmokerState:
    def __init__(self):
        #Movement data
        self.lmotor = 0
        self.rmotor = 0
        self.lspeed = 0
        self.rspeed = 0

        #sensor data. Set to high values for default.
        self.angle45 = 99999
        self.angle60 = 99999
        self.angle75 = 99999
        self.angle90 = 99999
        self.angle105 = 99999
        self.angle120 = 99999
        self.angle135 = 99999

    #take dictionary with keys and change the state of the class attributes
    #   to what the dictionary has.
    def setMotors(self, states:dict) -> None:
        for key in ["lmotor", "rmotor", "lspeed", "rspeed"]:
            try:
                t = states[key]
            
            #If one of the expected keys is missing, set the robot to its
            #   default state of not moving.
            except KeyError:
                print("Key does not exist: %s" % (key))
                self.rmotor = 0
                self.lmotor = 0
                self.lspeed = 0
                self.rspeed = 0
            
            #OTherwise, alter state
            else:
                self.lmotor = states["lmotor"]
                self.rmotor = states["rmotor"]
                self.lspeed = states["lspeed"]
                self.rspeed = states["rspeed"]

    def getState(self) -> dict:
        return {
            "lmotor":self.lmotor,
            "rmotor":self.rmotor,
            "lspeed":self.lspeed,
            "rspeed":self.rspeed
        }
    
    def setSensor(self, sens:dict) -> None:
        for key in ["angle45", "angle60", "angle75", "angle90", "angle105" ,"angle120", "angle135"]:
            try:
                t = sens[key]
            except KeyError:
                print("Key does not exist: %s" % (key))
                self.angle45 = 99999
                self.angle60 = 99999
                self.angle75 = 99999
                self.angle90 = 99999
                self.angle105 = 99999
                self.angle120 = 99999
                self.angle135 = 99999
            else: 
                self.angle45 = sens["angle45"]
                self.angle60 = sens["angle60"]
                self.angle75 = sens["angle75"]
                self.angle90 = sens["angle90"]
                self.angle105 = sens["angle90"]
                self.angle120 = sens["angle120"]
                self.angle135 = sens["angle135"] 
    
    def getSensor(self) -> dict:
        return {
            "angle45":self.angle45, 
            "angle60":self.angle60, 
            "angle75":self.angle75, 
            "angle90":self.angle90, 
            "angle105":self.angle105, 
            "angle120":self.angle120, 
            "angle135":self.angle135
        }
        
state = SmokerState()

@server.route("/smokerV2", methods=["POST", "GET"])
def mainEndpoint():
    if (request.method == "GET"):
        returnDict = state.getState()
        returnDict.update(state.getSensor())
        response = make_response(jsonify(returnDict))
        response.headers["Access-Control-Allow-Origin"] = "*"
        return response
    elif (request.method == "POST"):
        data = request.get_json(force=True)
        print(data)
        print()
        if (data["type"] == 1):
            readings = {
                "angle45":data["angle45"],
                "angle60":data["angle60"],
                "angle75":data["angle75"],
                "angle90":data["angle90"],
                "angle105":data["angle105"],
                "angle120":data["angle120"],
                "angle135":data["angle135"],
            }
            state.setSensor(readings)
            print("Attributes set")
        elif (data["type"][0] == 2):
            readings = {
                "lmotor":data["lmotor"][0],
                "rmotor":data["rmotor"][0],
                "lspeed":data["lspeed"][0],
                "rspeed":data["rspeed"][0]
            }
            state.setMotors(readings)
            print("Motors set")
            
        for k, v in state.__dict__.items():
            print("%s : %s" % (k, v))
        response = make_response({"status":"ok"})
        response.headers["Access-Control-Allow-Origin"] = "*"

        return response

if __name__ == "__main__":
    server.run(host="0.0.0.0", port=5000, debug=True)