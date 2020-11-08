#pragma once
#include "gamecommon.h"
#include "position.h"
#include "piece.h"

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

    static void Serialize(obytestream& stream, ICommand* command);
    static ICommand* Deserialize(ibytestream& stream);

    virtual Type GetType() const = 0;
	virtual std::string ToString() const = 0;
    virtual PieceType GetPieceType() const = 0;
	virtual Color GetPieceColor() const = 0;
	virtual bool DidCapture() const = 0;
    virtual PieceType GetCapturedPieceType() const = 0;
	virtual Color GetCapturedPieceColor() const = 0;
	virtual bool CheckEmptyEnemy() const = 0;
	virtual bool KingUnderAttak() const = 0;
	virtual void SetKingUnderAttak(bool king) = 0;
    virtual bool GetFlagChangable() const = 0;
    virtual bool IsFromReplication() const = 0;
    virtual void MarkFromReplication() = 0;

protected:

    virtual void Write(obytestream& stream) const = 0;
    virtual bool Read(ibytestream& stream) = 0;
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

    Type GetType() const override;
	std::string ToString() const override;
    PieceType GetPieceType() const override;
	Color GetPieceColor() const override;
	bool DidCapture() const override;
    PieceType GetCapturedPieceType() const override;
	Color  GetCapturedPieceColor() const override;
	bool CheckEmptyEnemy() const override;
	bool KingUnderAttak() const override;
	void SetKingUnderAttak(bool king) override
	{
		kingUnderAtak_=king;
	}

    bool GetFlagChangable() const override {return flagChangable;}
    void SetFlagChangable(bool flag_Changable)  {flagChangable=flag_Changable;}

    bool IsFromReplication() const override;
    void MarkFromReplication() override;

protected:

    void Write(obytestream& stream) const override;
    bool Read(ibytestream& stream) override;

	Pos posFrom_;
	Pos posTo_;
    PieceType pieceType_ = Chess::PieceType::Pawn;
    Color pieceColor_ = Color::White;
	bool pieceHasMovedBefore_ = false;
    bool kingUnderAtak_ = true;
    bool flagChangable = false;
	std::optional<SerializedPiece> enemyPiece_;
    bool fromReplication_ = false;
};

using ICommandUnPtr = std::unique_ptr<ICommand>;

}
