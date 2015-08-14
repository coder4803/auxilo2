/* scriptrunnerpool.hh
 * 
 * This header defines the ScriptRunnerPool class that works as a object pool /
 * constant builder class for the ScriptRunner objects.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 */

#ifndef SCRIPTRUNNERPOOL_HH
#define SCRIPTRUNNERPOOL_HH

#include "scriptrunner.hh"
#include <memory>
#include <vector>

namespace SignalHandler
{

/*!
 * \brief The ScriptRunnerPool class
 *  This class works as a object pool / constant builder for ScriptRunners.
 */
class ScriptRunnerPool
{
public:

    /*!
     * \brief Constructor.
     * \param queue SignalQueue that ScriptRunners will be constructed with.
     * \param library ScriptLibrary that ScriptRunners are constructed with.
     * \param interpreter_pool ScriptLangWrapper pool that ScriptRunners are
     *  constructed with.
     * \pre queue_ != nullptr, library != nullptr, interpreter_pool != nullptr.
     */
    ScriptRunnerPool(std::shared_ptr<SignalQueue> queue,
                     const ScriptLibrary* library,
                     std::shared_ptr<ScriptInterpreterPool> interpreter_pool);
    
    /*!
     * \brief Destructor.
     */
    ~ScriptRunnerPool();
    
    /*!
     * \brief Get new / re-used ScriptRunner.
     * \return Exclusively usable ScriptRunner.
     * \pre None.
     * \post Note that ScriptLibrary and ScriptRunner's subject may be outdated.
     *  Update thease before using reserved object.
     */
    std::unique_ptr<ScriptRunner> reserve();
    
    /*!
     * \brief release Release reserved ScriptRunner for others to use.
     * \param runner Released ScriptRunner.
     * \pre runner has been stopped.
     * \post runner is waiting in the pool until it is reserved again.
     */
    void release(std::unique_ptr<ScriptRunner>&& runner);
    
    
private:
    
    std::vector<std::unique_ptr<ScriptRunner> > runners_;
    std::shared_ptr<SignalQueue> queue_;
    const ScriptLibrary* library_;
    std::shared_ptr<ScriptInterpreterPool> wrapper_pool_;
};


}

#endif // SCRIPTRUNNERPOOL_HH
