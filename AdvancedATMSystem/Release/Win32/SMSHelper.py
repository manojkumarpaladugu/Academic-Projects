import way2sms

q=way2sms.Sms()

def SignIn(username, password):
	q.init(username, password)
	
def SendSMS(mobileNumber, message):
	q.send(str(mobileNumber), str(message))      # both are STRING
	
def SignOut():
	q.logout()