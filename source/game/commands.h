#pragma once
#include "gamecommon.h"
#include "position.h"
#include "piece.h"
#include <optional>
#include <QPixmap>

namespace Chess
{

class ICommand
{
public:

	enum class Type : char
	{
		Move,
		Promote,
		Compound
	};

	virtual ~ICommand() {}

	virtual bool Validate(const Game& game, bool flag_changable=false) const = 0;
	virtual void Do(Game& game, bool flag_changable=false) = 0;
	virtual void Undo(Game& game, bool flag_changable=false) = 0;

	static std::vector<char> Serialize(ICommand* command);
	static ICommand* Deserialize(const std::vector<char>& data);

	virtual std::string ToString() const = 0;
	virtual Type GetPieceType() const = 0;
	virtual Color GetPieceColor() const = 0;
	virtual bool DidCapture() const = 0;
	virtual Type GetCapturedPieceType() const = 0;
	virtual Color GetCapturedPieceColor() const = 0;
	virtual bool CheckEmptyEnemy() const = 0;
	virtual bool KingUnderAttak() const = 0;
	virtual void SetKingUnderAttak(bool king) = 0;
	virtual bool GetFlagChangable()=0;

protected:

	virtual std::vector<char> Write() const = 0;
	virtual void Read(const std::vector<char>& data) = 0;
};


class MoveCommand : public ICommand
{
public:

	MoveCommand();

	MoveCommand(Piece& piece, const Pos& pos);
	MoveCommand(const Pos& pos_from, const Pos& pos_to);

	bool Validate(const Game& game, bool flag_changable=false) const override;
	void Do(Game& game, bool flag_changable=false) override;
	void Undo(Game& game, bool flag_changable=false) override;

	std::string ToString() const override;
	Type GetPieceType() const override;
	Color GetPieceColor() const override;
	bool DidCapture() const override;
	Type GetCapturedPieceType() const override;
	Color  GetCapturedPieceColor() const override;
	bool CheckEmptyEnemy() const override;
	bool KingUnderAttak() const override;
	void SetKingUnderAttak(bool king) override
	{
		kingUnderAtak_=king;
	}

	bool GetFlagChangable() override {return flagChangable;}
	void SetFlagChangable(bool flag_Changable)  {flagChangable=flag_Changable;}

protected:

	std::vector<char> Write() const override;
	void Read(const std::vector<char>& data) override;

	Pos posFrom_;
	Pos posTo_;
	Type pieceType_ = Type::Pawn;
	Color pieceColor_=Color::White;
	bool pieceHasMovedBefore_ = false;
	bool kingUnderAtak_=true;
	bool flagChangable=false;
	std::optional<SerializedPiece> enemyPiece_;
};

using ICommandUnPtr = std::unique_ptr<ICommand>;

}
