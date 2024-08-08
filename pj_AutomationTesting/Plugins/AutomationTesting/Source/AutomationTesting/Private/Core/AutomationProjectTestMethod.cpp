// Copyright (C) RenZhai.2024.All Rights Reserved.

#include "Core/AutomationProjectTestMethod.h"

namespace AutomationProjectTestMethod
{
	FKey KeyCodeToFKey(int32 KeyCode)
	{
		switch(KeyCode)
		{
			 //NUM KEY
			case 48:case 96: return EKeys::Zero;
			case 49:case 97: return EKeys::One;
			case 50:case 98: return EKeys::Two;
			case 51:case 99: return EKeys::Three;
			case 52:case 100: return EKeys::Four;
			case 53:case 101: return EKeys::Five;
			case 54:case 102: return EKeys::Six;
			case 55:case 103: return EKeys::Seven;
			case 56:case 104: return EKeys::Eight;
			case 57:case 105: return EKeys::Nine;

			//letter key
			case 65: return EKeys::A;
			case 66: return EKeys::B;
			case 67: return EKeys::C;
			case 68: return EKeys::D;
			case 69: return EKeys::E;
			case 70: return EKeys::F;
			case 71: return EKeys::G;
			case 72: return EKeys::H;
			case 73: return EKeys::I;
			case 74: return EKeys::J;
			case 75: return EKeys::K;
			case 76: return EKeys::L;
			case 77: return EKeys::M;
			case 78: return EKeys::N;
			case 79: return EKeys::O;
			case 80: return EKeys::P;
			case 81: return EKeys::Q;
			case 82: return EKeys::R;
			case 83: return EKeys::S;
			case 84: return EKeys::T;
			case 85: return EKeys::U;
			case 86: return EKeys::V;
			case 87: return EKeys::W;
			case 88: return EKeys::X;
			case 89: return EKeys::Y;
			case 90: return EKeys::Z;

			//math operation
			case 106: return EKeys::Multiply;
			case 107: return EKeys::Add;
			case 108: return EKeys::Enter;
			case 109: return EKeys::Subtract;
			case 110: return EKeys::Decimal;
			case 111: return EKeys::Divide;

			//F key
			case 112: return EKeys::F1;
			case 113: return EKeys::F2;
			case 114: return EKeys::F3;
			case 115: return EKeys::F4;
			case 116: return EKeys::F5;
			case 117: return EKeys::F6;
			case 118: return EKeys::F7;
			case 119: return EKeys::F8;
			case 120: return EKeys::F9;
			case 121: return EKeys::F10;
			case 122: return EKeys::F11;
			case 123: return EKeys::F12;

			//Control key
			case 8: return EKeys::BackSpace;
			case 9: return EKeys::Tab;
			case 13: return EKeys::Enter;
			case 16: return EKeys::LeftShift; 
			case 17: return EKeys::LeftControl;
			case 18: return EKeys::LeftAlt;

			case 20: return EKeys::CapsLock;
			case 27: return EKeys::Escape;
			case 32: return EKeys::SpaceBar;
			case 33: return EKeys::PageUp;
			case 34: return EKeys::PageDown;
			case 35: return EKeys::End;
			case 36: return EKeys::Home;
			case 45: return EKeys::Insert;
			case 46: return EKeys::Delete;
			case 144: return EKeys::NumLock;


			/*  case 177: return EKeys::MediaPreviousTrack;
			case 176: return EKeys::MediaNextTrack;
			case 179: return EKeys::MediaPlayPause;
			case 175: return EKeys::MediaVolumeUp;
			case 174: return EKeys::MediaVolumeDown;*/

			default: return EKeys::Invalid;
		}
	}

	int32 FKeyToKeyCode(const FKey& Key)
	{

		if (Key == EKeys::Zero) return 48;
		if (Key == EKeys::One) return 49;
		if (Key == EKeys::Two) return 50;
		if (Key == EKeys::Three) return 51;
		if (Key == EKeys::Four) return 52;
		if (Key == EKeys::Five) return 53;
		if (Key == EKeys::Six) return 54;
		if (Key == EKeys::Seven) return 55;
		if (Key == EKeys::Eight) return 56;
		if (Key == EKeys::Nine) return 57;

  
		if (Key == EKeys::A) return 65;
		if (Key == EKeys::B) return 66;
		if (Key == EKeys::C) return 67;
		if (Key == EKeys::D) return 68;
		if (Key == EKeys::E) return 69;
		if (Key == EKeys::F) return 70;
		if (Key == EKeys::G) return 71;
		if (Key == EKeys::H) return 72;
		if (Key == EKeys::I) return 73;
		if (Key == EKeys::J) return 74;
		if (Key == EKeys::K) return 75;
		if (Key == EKeys::L) return 76;
		if (Key == EKeys::M) return 77;
		if (Key == EKeys::N) return 78;
		if (Key == EKeys::O) return 79;
		if (Key == EKeys::P) return 80;
		if (Key == EKeys::Q) return 81;
		if (Key == EKeys::R) return 82;
		if (Key == EKeys::S) return 83;
		if (Key == EKeys::T) return 84;
		if (Key == EKeys::U) return 85;
		if (Key == EKeys::V) return 86;
		if (Key == EKeys::W) return 87;
		if (Key == EKeys::X) return 88;
		if (Key == EKeys::Y) return 89;
		if (Key == EKeys::Z) return 90;

 
		if (Key == EKeys::F1) return 112;
		if (Key == EKeys::F2) return 113;
		if (Key == EKeys::F3) return 114;
		if (Key == EKeys::F4) return 115;
		if (Key == EKeys::F5) return 116;
		if (Key == EKeys::F6) return 117;
		if (Key == EKeys::F7) return 118;
		if (Key == EKeys::F8) return 119;
		if (Key == EKeys::F9) return 120;
		if (Key == EKeys::F10) return 121;

   
		if (Key == EKeys::Semicolon) return 59;
		if (Key == EKeys::LeftShift) return 16; 

		if (Key == EKeys::BackSpace) return 8;
		if (Key == EKeys::Tab) return 9;
		if (Key == EKeys::Enter) return 13;
		if (Key == EKeys::LeftShift) return 16; 
		if (Key == EKeys::LeftControl) return 17;
		if (Key == EKeys::LeftAlt) return 18;

		if (Key == EKeys::CapsLock) return 20;
		if (Key == EKeys::Escape) return 27;
		if (Key == EKeys::SpaceBar) return 32;
		if (Key == EKeys::PageUp) return 33;
		if (Key == EKeys::PageDown) return 34;
		if (Key == EKeys::End) return 35;
		if (Key == EKeys::Home) return 36;
		if (Key == EKeys::Insert) return 45;
		if (Key == EKeys::Delete) return 46;
		if (Key == EKeys::NumLock) return 144;

		return -1; 
	}

	FKey GetFKeyFromCharCode(TCHAR CharCode)
	{
		FString KeyName = FString::Printf(TEXT("%c"), CharCode);
		return FKey(*KeyName);
	}

	FKey GetMouseKey(EMouseButtons::Type InType)
	{
		switch (InType)
		{
		case EMouseButtons::Left:
			return EKeys::LeftMouseButton;
		case EMouseButtons::Middle:
			return EKeys::MiddleMouseButton;
		case EMouseButtons::Right:
			return EKeys::RightMouseButton;
		case EMouseButtons::Thumb01:
			return EKeys::ThumbMouseButton;
		case EMouseButtons::Thumb02:
			return EKeys::ThumbMouseButton2;
		}

		return EKeys::AnyKey;
	}
}