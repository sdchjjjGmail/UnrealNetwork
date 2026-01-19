// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/TestMultiplayGameInstance.h"
#include "Kismet/GameplayStatics.h"

UTestMultiplayGameInstance::UTestMultiplayGameInstance()
{
	ServerIP = "127.0.0.1";
}

void UTestMultiplayGameInstance::Init()
{
	Super::Init();
	if (UEngine* Engine = GetEngine())
	{
		// 게임 플레이 중 연결이 끊어졌다. 핑이 너무 길다. 네트워크 드라이버에 문제가 생겼다.
		Engine->OnNetworkFailure().AddUObject(this, &UTestMultiplayGameInstance::HandleNetworkFailure);

		// 서버 주소가 잘못됬다, 서버 인원이 꽉 찼다, 맵 파일이 없다.
		Engine->OnTravelFailure().AddUObject(this, &UTestMultiplayGameInstance::HandleTravelFailure);
	}
}

void UTestMultiplayGameInstance::CreateServer()
{
	FString MapName;
	if (!ServerLevelAsset.IsNull())
	{
		MapName = ServerLevelAsset.GetLongPackageName();	// 레벨 에셋에서 맵 경로 추출
	}
	else
	{
		MapName = UGameplayStatics::GetCurrentLevelName(GetWorld());	// 거의 의미없음.
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("World가 없습니다!!! 서버를 만들 수 없습니다."));
		return;
	}

	FString Options = FString::Printf(TEXT("listen?MaxPlayers=%d"), MaxPlayers);
	UE_LOG(LogTemp, Log, TEXT("리슨서버 생성 : %s, 최대인원 : %d"), *MapName, MaxPlayers);

	UGameplayStatics::OpenLevel(World, FName(*MapName), true, Options);
	UE_LOG(LogTemp, Log, TEXT("리슨서버 시작"));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, TEXT("리슨서버 시작"));
	}
}

void UTestMultiplayGameInstance::JoinServer(FString IPAddress)
{
	if (IPAddress.IsEmpty())
	{
		IPAddress = ServerIP;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("World가 없습니다!!! 서버에 접속할 수 없습니다."));
		return;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController가 없습니다!!! 서버에 접속할 수 없습니다."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("서버에 접속합니다 : %s"), *IPAddress);
	PC->ClientTravel(IPAddress, ETravelType::TRAVEL_Absolute);
	
	UE_LOG(LogTemp, Log, TEXT("서버 접속 시작"));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, TEXT("서버 접속 시작"));
	}
}

void UTestMultiplayGameInstance::DisconnectServer()
{
	FString MapName;
	if (!MainLevelAsset.IsNull())
	{
		MapName = MainLevelAsset.GetLongPackageName();	// 레벨 에셋에서 맵 경로 추출
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MainLevelAsset이 없습니다!!! 맵을 이동할 수 없습니다."));
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("World가 없습니다!!!"));
		return;
	}
	APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController가 없습니다!!!"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("서버에서 나가는 중입니다..."));
	PC->ClientTravel(MapName, ETravelType::TRAVEL_Absolute);

	UE_LOG(LogTemp, Log, TEXT("서버 접속 해제"));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, TEXT("서버 접속 해제"));
	}
}

void UTestMultiplayGameInstance::HandleNetworkFailure(
	UWorld* World, UNetDriver* NetDirever, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	UE_LOG(LogNet, Error, TEXT("HandleNetworkFailure 실행"));
	UE_LOG(LogNet, Error, TEXT("오류 타입 : %d"), (int32)FailureType);
	UE_LOG(LogNet, Error, TEXT("오류 메세지 : %s"), *ErrorString);

	//switch (FailureType)
	//{
	//case ENetworkFailure::NetDriverAlreadyExists:
	//case ENetworkFailure::NetDriverCreateFailure:
	//case ENetworkFailure::NetDriverListenFailure:
	//case ENetworkFailure::ConnectionLost:
	//case ENetworkFailure::ConnectionTimeout:
	//case ENetworkFailure::FailureReceived:
	//case ENetworkFailure::OutdatedClient:
	//case ENetworkFailure::OutdatedServer:
	//case ENetworkFailure::PendingConnectionFailure:
	//case ENetworkFailure::NetGuidMismatch:
	//case ENetworkFailure::NetChecksumMismatch:
	//default:
	//	break;
	//}
}

void UTestMultiplayGameInstance::HandleTravelFailure(
	UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorString)
{
	UE_LOG(LogNet, Error, TEXT("HandleTravelFailure 실행"));
	UE_LOG(LogNet, Error, TEXT("오류 타입 : %d"), (int32)FailureType);
	UE_LOG(LogNet, Error, TEXT("오류 메세지 : %s"), *ErrorString);

	switch (FailureType)
	{
	case ETravelFailure::ServerTravelFailure:
		if (ErrorString.Contains(TEXT("Full")) || ErrorString.Contains(TEXT("full")))
		{
			UE_LOG(LogNet, Error, TEXT("서버가 가득차 접속이 거부되었다."));
		}
		break;
	default:
		break;
	}
}