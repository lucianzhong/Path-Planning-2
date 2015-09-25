#pragma once

#include "helpers.hpp"
#include "priority_queue.hpp"

namespace search
{
    namespace ds
    {
        class DstarCore
        {
            //
            //  Algorithm
            //

            auto validate(Cell c) const
            {
                return c.row >= 0 && c.row < (int)matrix.rows() && c.col >= 0 && c.col < (int)matrix.cols();
            }
            auto valid_neighbours_of(Cell c) const
            {
                Cells neighbours;
                for (auto direction = '1'; direction != '9'; ++direction)
                {
                    auto n = DIRECTIONS.at(direction)(c);
                    if (validate(n))
                        neighbours.insert(n);
                }
                return neighbours;
            }
            auto initialize()
            {
                q.reset();
                km = at(goal).r = 0;
                q.push(goal);
            }

            //
            //  helpers
            //

            auto at(Cell c) const -> LpState const&
            {
                return matrix.at(c);
            }
            auto at(Cell c) -> LpState&
            {
                return matrix.at(c);
            }
            auto mark_bad_cells(Cells const& bad_cells)
            {
                for (auto c : bad_cells) at(c).bad = true;
            }
            auto mark_h_values_with(Cell terminal)
            {
                auto mark_h = [=](Cell c) { at(c).h = hfunc(c, terminal); };
                matrix.each_cell(mark_h);
            }
            auto reset_statistics()
            {
                run_time = max_q_size = 0;
                expansions.clear(), path.clear();
            }

        public:
            //
            //  Constructor
            //
            DstarCore(unsigned rows, unsigned cols, Cell start, Cell goal, string heuristic, Cells const& bad_cells) :
                matrix{ rows, cols },
                start{ start },
                goal{ goal },
                hfunc{ HEURISTICS.at(heuristic) },
                km{ 0 },
                q{ [this](Cell l, Cell r) { return Key{ at(l), km } < Key{ at(r), km }; } }
            {
                mark_bad_cells(bad_cells);
                mark_h_values_with(start);  //h value : start to current
                reset_statistics();
            }
            //
            //  data members
            //
            Matrix matrix;
            Cell const start, goal;
            function<int(Cell, Cell)> const hfunc;
            int km;
            PriorityQueue < Cell, function<bool(Cell, Cell)> > q;
            //
            //  statistics
            //
            size_t max_q_size;
            Cells expansions;
            string path;
            long long run_time;
        };
    }
}
