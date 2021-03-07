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
        Replace,
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

using ICommandUnPtr = std::unique_ptr<ICommand>;

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
	std::optional<SerializedPiece> enemyPiece_;
    bool fromReplication_ = false;
};

class ReplacePieceCommand : public ICommand
{
public:

    ReplacePieceCommand(Pos piece, PieceType replace_type);

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

    Pos pos_;
    PieceType replaceType_;
    PieceType replacedType_;
    bool isFromReplication_ = false;
};

class CompoundCommand : public ICommand
{
public:

    CompoundCommand(std::vector<ICommandUnPtr>&& commands);

    virtual bool Validate(const Game& game) const override;
    virtual void Do(Game& game) override;
    virtual void Undo(Game& game) override;

    virtual Type GetType() const override;
    virtual std::string ToString() const override;
    virtual bool IsFromReplication() const override;
    virtual void MarkFromReplication() override;

protected:

    virtual void Write(obytestream& stream) const override;
    virtual bool Read(ibytestream& stream) override;

    std::vector<ICommandUnPtr> commands_;
    bool fromReplication_ = false;
};

}
