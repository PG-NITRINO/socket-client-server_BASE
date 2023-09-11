#include "Responses.h"

std::string FormationResponse(QueryResult& queryResult)
{
	std::string response;

	switch (queryResult.queryType)
	{
		case (QueryType::registration):
		{
			response = "registration/";

			switch (queryResult.returnCode)
			{
				case (ReturnCode::success):			response += '0'; break;
				case (ReturnCode::userExists):		response += '1'; break;
				case (ReturnCode::shortPassword):	response += '2'; break;
				case (ReturnCode::unknowError): 
				default: response += '3';
			}
			break;
		}

		case (QueryType::authentication):
		{
			response = "authentication/";
			switch (queryResult.returnCode)
			{
			case (ReturnCode::authentificationFalse):	response += '4'; break;
			case (ReturnCode::success):
			{
				response += "0/";
				response += queryResult.login;
				response += "&";
				response += queryResult.jwt.jwt;
				
				break;
			}
			case (ReturnCode::unknowError):
			default: response += '3';
			}
			break;
		}

		default: {
			response = "Not Found";
			break;
		}
	}

	return response;
}