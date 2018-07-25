#include "RpcHandler.h"

#include "Methods/RpcMethods.h"

#define ADD_METHOD(x) insert(#x, new x())

RpcHandler::RpcHandler()
{
	this->_builtinMethods.ADD_METHOD(GetVersion);
}

RpcHandler::~RpcHandler() = default;

const RpcResponse RpcHandler::processCall(const RpcRequest& request)
{
	const QString& methodName = request.getMethodName();

	// Try to resolve the method from the built-in ones
	RpcMethod* method = this->_builtinMethods.value(methodName, nullptr);
	if (method) {
		return method->handle(request);
	} else {
		// Otherwise let's try from the 3rd party ones
		RpcMethod* thirdPartyMethod =
			this->_thirdPartyMethods.value(methodName, nullptr);
		if (thirdPartyMethod) {
			return thirdPartyMethod->handle(request);
		}
	}

	return RpcResponse::fail(request, "Method not found",
							 RpcErrorCode::MethodNotFound);
}