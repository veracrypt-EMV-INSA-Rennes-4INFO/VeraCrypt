#include "Token.h"
#include "Platform/Finally.h"
#include "Platform/ForEach.h"

#if !defined(TC_WINDOWS) || defined(TC_PROTOTYPE)
#include "Platform/SerializerFactory.h"
#include "Platform/StringConverter.h"
#include "Platform/SystemException.h"
#else
#include "Dictionary.h"
#include "Language.h"
#endif

#include <vector>
#include <algorithm>
#include <memory>

#include "SecurityToken.h"
#include "EMVToken.h"
#include "iostream"

using namespace std;

namespace VeraCrypt
{
	vector<shared_ptr<TokenKeyfile>> Token::GetAvailableKeyfiles() {
		//vector<SecurityTokenKeyfile> v1 = SecurityToken::GetAvailableKeyfiles();
		vector<EMVTokenKeyfile> v2 = EMVToken::GetAvailableKeyfiles();
		vector<shared_ptr<TokenKeyfile>> v_ptr;

		/*foreach (SecurityTokenKeyfile k, v1) {
			v_ptr.push_back(shared_ptr<TokenKeyfile>(new SecurityTokenKeyfile(k)));
		}*/

		foreach (EMVTokenKeyfile k, v2) {
			v_ptr.push_back(shared_ptr<TokenKeyfile>(new EMVTokenKeyfile(k)));
		}

		return v_ptr;
	}

	void Token::GetKeyfileData(const shared_ptr<TokenKeyfile> keyfile, vector<byte>& keyfileData)
	{
		shared_ptr<SecurityTokenKeyfile> securityTokenKeyfile = dynamic_pointer_cast<SecurityTokenKeyfile>(keyfile);
		if(securityTokenKeyfile){
			SecurityToken::GetKeyfileData(*securityTokenKeyfile.get(),keyfileData);
		} else {
			shared_ptr<EMVTokenKeyfile> emvTokenKeyfile = dynamic_pointer_cast<EMVTokenKeyfile>(keyfile);
			if(emvTokenKeyfile){
				EMVToken::GetKeyfileData(*emvTokenKeyfile.get(),keyfileData);
			} 
		}
	}

	bool Token::IsKeyfilePathValid(const wstring& tokenKeyfilePath)
	{
		return SecurityToken::IsKeyfilePathValid(tokenKeyfilePath) || EMVToken::IsKeyfilePathValid(tokenKeyfilePath);
	}

	list <shared_ptr<TokenInfo>> Token::GetAvailableTokens()
	{
		list <shared_ptr<TokenInfo>> availableTokens;
		foreach(SecurityTokenInfo securityToken, SecurityToken::GetAvailableTokens()){
			availableTokens.push_back(shared_ptr<TokenInfo>(new SecurityTokenInfo(std::move(securityToken))));
		}

		return availableTokens ;
	}

	shared_ptr<TokenKeyfile> Token::getTokenKeyfile(const TokenKeyfilePath path){
		shared_ptr<TokenKeyfile> tokenKeyfile;

		if(SecurityToken::IsKeyfilePathValid(path)){
			tokenKeyfile = shared_ptr<TokenKeyfile>(new SecurityTokenKeyfile(path));
		} else {
			if(EMVToken::IsKeyfilePathValid(path)){
				tokenKeyfile = shared_ptr<TokenKeyfile>(new EMVTokenKeyfile(path));
			}		
		}

		return tokenKeyfile;
	}
}
