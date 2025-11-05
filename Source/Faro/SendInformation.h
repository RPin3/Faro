

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SendInformation.generated.h"


UINTERFACE()
class USendInformation : public UInterface
{
	GENERATED_BODY()
};

class FARO_API ISendInformation
{
	GENERATED_BODY()

	public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Comunicación")
	void ReciveInformation(AActor* object);
};
