from flask import Flask, request, jsonify
from flask_sqlalchemy import SQLAlchemy
from flask_restful import Resource, Api, reqparse, fields, marshal_with, abort
from flask_cors import CORS, cross_origin

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = "sqlite:///database.db"
db=SQLAlchemy(app)
api = Api(app)
CORS(app)

live_feed_data = "Nigger"
class UserModel(db.Model):
    id = db.Column(db.Integer, primary_key=True,nullable=False)
    name = db.Column(db.String(80), unique=True,nullable=False)
    email = db.Column(db.String(80), unique=True,nullable=False)
    password = db.Column(db.String(80), unique=True,nullable=False)
    
    def __repr__(self):
        return f"User(name={self.name},email={self.email},password={self.password})"

user_args = reqparse.RequestParser()
user_args.add_argument('name',type=str,required=True,help="Name Cannot Be Blank")
user_args.add_argument('email',type=str,required=True,help="Email Cannot Be Blank")
user_args.add_argument('password',type=str,required=True,help="Password Cannot Be Blank")


userFields = {
    'id': fields.Integer,
    'name': fields.String,
    'email': fields.String,
    'password':fields.String,
}
dataFields = {
    'id': fields.Integer,
    'time_log': fields.String,
    'color_log': fields.String,
    'gas_log': fields.String,
    'temp_log': fields.String,
}
class Users(Resource):
    @marshal_with(userFields)
    def get(self):
        users = UserModel.query.all()
        return users
    @marshal_with(userFields)
    def post(self):
        args = user_args.parse_args()
        user = UserModel(name=args['name'],email=args['email'],password=args['password'])
        db.session.add(user)
        db.session.commit()
        users = UserModel.query.all()
        return users, 201

class User(Resource):
    @marshal_with(userFields)
    def get(self,name,password):
        user = UserModel.query.filter_by(name=name,password=password).first()
        if not user:
            abort(404,"User not found")
        return user
    

class DataLog(db.Model):
    id = db.Column(db.Integer, primary_key=True,nullable=False)
    time_log = db.Column(db.String(80), unique=False,nullable=False)
    color_log = db.Column(db.String(80), unique=False,nullable=False)
    gas_log = db.Column(db.String(80), unique=False,nullable=False)
    temp_log = db.Column(db.String(80), unique=False,nullable=True)
    
    def __repr__(self):
        return f"Data(time={self.time_log},color={self.color_log},gas={self.gas_log},temp={self.temp_log})"
    
data_args = reqparse.RequestParser()
data_args.add_argument('time_log',type=str,required=True,help="Time Cannot Be Blank")
data_args.add_argument('color_log',type=str,required=True,help="Color Cannot Be Blank")
data_args.add_argument('gas_log',type=str,required=True,help="Gas Cannot Be Blank")
data_args.add_argument('temp_log',type=str,required=True,help="Temp Cannot Be Blank")

class Data(Resource):
    @marshal_with(dataFields)
    
    def get(self):
        data = DataLog.query.all()
        return data
    @marshal_with(dataFields)
    def post(self):
        args = data_args.parse_args()
        data = DataLog(time_log=args['time_log'],color_log=args['color_log']
                         ,gas_log=args['gas_log'],temp_log=args['temp_log'])
        db.session.add(data)
        db.session.commit()
        datas = DataLog.query.all()
        return datas, 201

class LiveData(Resource):  
    def get(self):
        global live_feed_data
        data = live_feed_data
        return data, 200
    def post(self):
        global live_feed_data
        live_feed_data = request.get_json()
        return {"status": "ok"}, 200

        

api.add_resource(Users,'/api/users/')
api.add_resource(User,'/api/user/<string:name>/<string:password>')
api.add_resource(Data,'/api/data/')
api.add_resource(LiveData,'/api/live/')

@app.route("/")
@cross_origin()
def home():
    return '<h1>FLASK API</h1>>'

if __name__ == "__main__":


    app.run(debug=True,host="0.0.0.0")