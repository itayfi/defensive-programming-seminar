class UserException(Exception):
	def __init__(self, message, status=500):
		super().__init__(message, status)
		self.message = message
		self.status = status
