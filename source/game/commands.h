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

    virtual bool Validate(const Game& game) const = 0;
    virtual void Do(Game& game) = 0;
    virtual void Undo(Game& game) = 0;

    static void Serialize(obytestream& stream, ICommand* command);
    static ICommand* Deserialize(ibytestream& stream);

    virtual Type GetType() const = 0;
	virtual std::string ToString() const = 0;
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

    bool Validate(const Game& game) const override;
    void Do(Game& game) override;
    void Undo(Game& game) override;

    Type GetType() const override;
	std::string ToString() const override;

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
	std::optional<SerializedPiece> enemyPiece_;
    bool fromReplication_ = false;
};

using ICommandUnPtr = std::unique_ptr<ICommand>;

}
