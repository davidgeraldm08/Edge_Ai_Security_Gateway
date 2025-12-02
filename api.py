from flask import Flask, request, jsonify
from flask_sqlalchemy import SQLAlchemy
from flask_restful import Resource, Api, reqparse, fields, marshal_with, abort



app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = "sqlite:///database.db"
db=SQLAlchemy(app)
api = Api(app)

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


userFields = {
    'id': fields.Integer,
    'name': fields.String,
    'email': fields.String
}

class Users(Resource):
    @marshal_with(userFields)
    def get(self):
        users = UserModel.query.all()
        return users
    @marshal_with(userFields)
    def post(self):
        args = user_args.parse_args()
        user = UserModel(name=args['name'],email=args['email'])
        db.session.add(user)
        db.session.commit()
        users = UserModel.query.all()
        return users, 201

api.add_resource(Users,'/api/users/')

class DataLog(db.Model):
    id = db.Column(db.Integer, primary_key=True,nullable=False)
    time_log = db.Column(db.String(80), unique=False,nullable=False)
    color_log = db.Column(db.String(80), unique=False,nullable=False)
    gas_Log = db.Column(db.Integer, unique=False,nullable=False)
    temp_log = db.Column(db.String(80), unique=False,nullable=True)
    
    def __repr__(self):
        return f"Data(time={self.time_log},color={self.color_log},gas={self.gas_log},temp={self.temp_log})"

@app.route("/")
def home():
    return '<h1>FLASK API</h1>>'

if __name__ == "__main__":


    app.run(debug=True,host="0.0.0.0")